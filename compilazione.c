#include "struct.h"

/* ms < 0: aspetta un rilascio; ms > 0: aspetta al massimo ms */
static void	attendi(t_sim *sim, long long ms)
{
	struct timeval	ora;
	struct timespec	scadenza;
	long long		usec;

	if (ms < 0)
	{
		pthread_cond_wait(&sim->queue_cond, &sim->queue_mutex);
		return ;
	}
	gettimeofday(&ora, NULL);
	usec = ora.tv_usec + ms * 1000;
	scadenza.tv_sec = ora.tv_sec + usec / 1000000;
	scadenza.tv_nsec = (usec % 1000000) * 1000;
	pthread_cond_timedwait(&sim->queue_cond, &sim->queue_mutex, &scadenza);
}

/*
** Aggiorna ultimo_avvio_compilazione e stampa i log in modo atomico
** rispetto al monitor, cosi un "burned out" non puo incrociarsi.
*/
static int	inizia_compilazione(t_coder *coder)
{
	t_sim		*sim;
	long long	ora;
	int			ok;

	sim = coder->sim;
	pthread_mutex_lock(&sim->print_mutex);
	pthread_mutex_lock(&sim->stato_mutex);
	ok = !sim->sim_finita;
	if (ok)
	{
		ora = get_time();
		coder->ultimo_avvio_compilazione = ora;
		ora -= sim->start_time;
		printf("%lld %d has taken a dongle\n", ora, coder->id);
		printf("%lld %d has taken a dongle\n", ora, coder->id);
		printf("%lld %d is compiling\n", ora, coder->id);
	}
	pthread_mutex_unlock(&sim->stato_mutex);
	pthread_mutex_unlock(&sim->print_mutex);
	return (ok);
}

void	esegui_compilazione(t_coder *coder)
{
	t_sim		*sim;
	long long	esito;

	sim = coder->sim;
	pthread_mutex_lock(&sim->queue_mutex);
	entra_in_coda(sim, coder);
	esito = prova_a_prendere_dongles(coder);
	while (esito != 0 && !simulazione_finita(sim))
	{
		attendi(sim, esito);
		esito = prova_a_prendere_dongles(coder);
	}
	pthread_mutex_unlock(&sim->queue_mutex);
	if (esito != 0)
		return ;
	if (inizia_compilazione(coder))
	{
		ft_usleep(sim, sim->tempo_comp);
		pthread_mutex_lock(&sim->stato_mutex);
		coder->num_compilazioni++;
		pthread_mutex_unlock(&sim->stato_mutex);
	}
	rilascia_dongles(coder);
}
