#include "struct.h"
#include <stdio.h>
#include <pthread.h>
#include <sys/time.h>





int main(int argc, char **argv)
{
    t_sim sim;
    int i;
    pthread_t thread_monitor;

    if (controllo_argomenti(argc, argv, &sim) != 0)
        return (1);

    if (allocazione_memoria(&sim) != 0)
        return (1);

    if (inizializzazione_dati(&sim) != 0)
        return (1);

    i = 0;
    while (i < sim.num_coders)
    {
        if (pthread_create(&sim.array_coders[i].thread, NULL, organizza, &sim.array_coders[i]) != 0)
        {
            printf("Errore: impossibile creare il thread\n");
            return (1);
        }
        i++;
    }

    if (pthread_create(&thread_monitor, NULL, controllo_burnout, &sim) != 0)
    {
        printf("Errore: impossibile creare il monitor\n");
        return (1);
    }

    pthread_join(thread_monitor, NULL);

    i = 0; 
    while (i < sim.num_coders)
    {
        pthread_join(sim.array_coders[i].thread, NULL);
        i++;
    }


    pthread_mutex_destroy(&sim.print_mutex);
    pthread_mutex_destroy(&sim.queue_mutex);
    pthread_cond_destroy(&sim.queue_cond);

    i = 0;
    while (i < sim.num_coders)
    {
        pthread_mutex_destroy(&sim.array_dongles[i].mutex);
        pthread_cond_destroy(&sim.array_dongles[i].cond);
        i++;
    }

    free(sim.array_coders);
    free(sim.array_dongles);
    free(sim.coda_attesa);

    return (0);
}