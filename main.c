#include "struct.h"
#include <stdio.h>
#include <pthread.h>

void *organizza(void *arg)
{
    struct s_coder *coder = (struct s_coder *)arg;

    int counter = 1;

    while (1)
    {
        pthread_mutex_lock(&coder->dongle_sinistro->mutex);
        pthread_mutex_lock(&coder->dongle_destro->mutex);
        printf("Coder %d ha preso i dongles\n", coder->id);
        pthread_mutex_unlock(&coder->dongle_sinistro->mutex);
        pthread_mutex_unlock(&coder->dongle_destro->mutex);
        printf("Coder %d ha preso i dongles\n", coder->id);
        counter++;

        if (coder->sim->num_compilazioni != -1 && counter >= coder->sim->num_compilazioni)
        break;

    }


    return NULL;
}

int main(int argc, char **argv)
{
    t_sim sim;
    

    if (controlla_argomenti(argc, argv, &sim) != 0)
        return (1);

    if (allocazione_memoria(&sim) != 0)
        return (1);

    if (inizializzazione_dati(&sim) != 0)
        return (1);

    int i;   
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

    i = 0; 
    while (i < sim.num_coders)
    {
        pthread_join(sim.array_coders[i].thread, NULL);
        i++;
    }

    return (0);
}