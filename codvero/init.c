#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "struct.h"

int allocazione_memoria (t_sim *sim)
{
    sim->array_coders = (struct s_coder *)malloc(sim->num_coders * sizeof(struct s_coder));

    if (sim->array_coders == NULL)
        return 1;

    sim->array_dongles = (struct s_dongle *)malloc(sim->num_coders * sizeof(struct s_dongles));

     if (sim->array_dongles == NULL)
     {
        free(sim->array_coders);
        return 1;
     }
}


int inizializzazione_dati(t_sim *sim)
{
    pthread_mutex_init(&sim->print_mutex, NULL);

    // Nella tua struct t_sim hai inserito print_mutex. 
    // Questo ti servirà più avanti per evitare che i thread scrivano 
    // sul terminale nello stesso millisecondo accavallando i testi. 


    int i;
    i = 0;

    while (sim->num_coders > i)
    {
        sim->array_dongles[i].id = i;
        sim->array_dongles[i].is_locked = 0;

        pthread_mutex_init(&sim->array_dongles[i].mutex, NULL);
        pthread_cond_init(&sim->array_dongles[i].cond, NULL);
        i++;
    }

    i = 0;
    while (sim->num_coders > i)
    {
        sim->array_coders[i].id = i + 1;
        sim->array_coders[i].num_compilazioni = 0;
        sim->array_coders[i].sim = sim;
        sim->array_coders[i].dongle_sinistro = &sim->array_dongles[i];
        i++;

    }
}