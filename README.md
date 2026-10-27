*This project has been created as part of the 42 curriculum by <login>.*

# Codexion

## Description

Codexion is a concurrency simulation written in C with POSIX threads.
`number_of_coders` coders sit in a circle with as many USB dongles between them.
To compile, a coder needs both the dongle on their left and the one on their right.
After compiling they debug, then refactor, then try to compile again.
A coder burns out if they do not start compiling within `time_to_burnout` ms
of the start of their last compile (or of the simulation).

The simulation stops when a coder burns out or when every coder has compiled
at least `number_of_compiles_required` times.

## Instructions

```
make
./codexion number_of_coders time_to_burnout time_to_compile time_to_debug \
           time_to_refactor number_of_compiles_required dongle_cooldown scheduler
```

- every numeric argument must be a non-negative integer that fits in an `int`
  (`number_of_coders` must be at least 1);
- `scheduler` is `fifo` or `edf`.

Example: `./codexion 4 800 200 200 200 5 10 edf`

Rules: `make`, `make clean`, `make fclean`, `make re`.

## Blocking cases handled

- **Deadlock (Coffman's conditions).** A coder never holds one dongle while
  waiting for the other: both dongles are checked and taken together while
  holding `queue_mutex`, so the *hold and wait* condition can never occur.
  Mutexes are always taken in the same order (`queue_mutex` → dongle mutex,
  `print_mutex` → `stato_mutex`), so there is no circular wait between locks.
- **Starvation / fairness.** Every request goes into a priority queue
  (a binary min-heap, `heap.c`). A coder may take its dongles only if no request
  with a higher priority wants one of the same dongles. With `fifo` the
  priority is the arrival order, with `edf` it is
  `last_compile_start + time_to_burnout`; ties are broken by the lower coder id.
  Coders that do not compete for the same dongles can still compile in parallel.
- **Cooldown.** Each dongle stores the time it was released. If the coder is
  allowed to take its dongles but the cooldown is not over, it waits with
  `pthread_cond_timedwait` exactly for the remaining time (or until another
  release wakes it up) and keeps its place in the queue.
- **Single coder.** With one coder there is only one dongle: the coder takes it,
  can never compile and burns out.
- **Precise burnout detection.** A dedicated monitor thread checks every coder
  about every millisecond and prints the burnout well within 10 ms.
- **Log serialization.** All output goes through `print_mutex`; nothing is
  printed after the end of the simulation.

## Thread synchronization mechanisms

- `print_mutex` (`pthread_mutex_t`): serializes every log line.
- `stato_mutex`: protects `sim_finita`, each coder's `num_compilazioni` and
  `ultimo_avvio_compilazione`, which are written by coders and read by the
  monitor. When a coder starts compiling it holds `print_mutex` + `stato_mutex`
  while updating its timestamp and printing, and the monitor holds the same two
  locks while checking: a coder cannot log "is compiling" after its own
  "burned out" line, and the monitor never reads a half-updated timestamp.
- `queue_mutex` + `queue_cond` (`pthread_cond_t`): protect the waiting heap.
  Coders wait on `queue_cond` (`pthread_cond_wait`, or `pthread_cond_timedwait`
  during a cooldown); releasing dongles and the end of the simulation do
  `pthread_cond_broadcast` so waiting coders re-check their turn.
- one `pthread_mutex_t` per dongle: protects `is_locked` and the release time.

## Resources

- `man pthread_mutex_lock`, `man pthread_cond_wait`, `man pthread_cond_timedwait`
- E. W. Dijkstra, the Dining Philosophers problem
- Coffman, Elphick, Shoshani, *System Deadlocks* (1971)
- Binary heap / priority queue: any algorithms textbook (e.g. CLRS, chapter 6)

**AI usage:** AI was used to review the code, find concurrency bugs (data races,
lost wake-ups, missing stop condition) and help write this README. All the
code was read, tested and understood before submission.
