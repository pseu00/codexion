#include "struct.h"
#include <stdio.h>
#include <pthread.h>
#include <sys/time.h> 


void *controllo_burnout(void *arg)
{
    t_sim *sim = (t_sim *)arg;
    int i;

    while (sim->sim_finita == 0)
    {
        i = 0;
        while (i < sim->num_coders)
        {
            if ((get_time() - sim->array_coders[i].ultimo_avvio_compilazione) >= sim->tempo_burnout)
            {
                sim->sim_finita = 1;

                pthread_mutex_lock(&sim->print_mutex);
                printf("%lld %d burned out\n", get_time(), sim->array_coders[i].id);
                pthread_mutex_unlock(&sim->print_mutex);

                // IL MEGAFONO: Svegliamo tutti quelli bloccati in coda così possono uscire!
                pthread_mutex_lock(&sim->queue_mutex);
                pthread_cond_broadcast(&sim->queue_cond);
                pthread_mutex_unlock(&sim->queue_mutex);

                return (NULL);
            }
            i++;
        }
        ft_usleep(1); 
    }
    return (NULL);
}