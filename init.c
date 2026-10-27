#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "struct.h"

int allocazione_memoria(t_sim *sim)
{
    sim->array_coders = malloc(sim->num_coders * sizeof(t_coder));
    if (sim->array_coders == NULL)
        return (1);

    sim->array_dongles = malloc(sim->num_coders * sizeof(t_dongle));
    if (sim->array_dongles == NULL)
    {
        free(sim->array_coders);
        return (1);
    }

    sim->coda_attesa = malloc(sizeof(int) * sim->num_coders);
    if (!sim->coda_attesa)
    {
        free(sim->array_coders);
        free(sim->array_dongles);
        return (1);
    }
    
    return (0);
}


int inizializzazione_dati(t_sim *sim)
{
    int i;

    pthread_mutex_init(&sim->print_mutex, NULL);
    pthread_mutex_init(&sim->queue_mutex, NULL);
    pthread_cond_init(&sim->queue_cond, NULL);
    
    sim->sim_finita = 0;
    sim->quanti_in_coda = 0;

    i = 0;
    while (sim->num_coders > i)
    {
        sim->array_dongles[i].id = i;
        sim->array_dongles[i].is_locked = 0;
        
        // Diamo un orario iniziale valido al dongle!
        sim->array_dongles[i].libero = get_time();
        
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
        
        // Diamo l'orologio di nascita vitale per il Monitor!
        sim->array_coders[i].ultimo_avvio_compilazione = get_time();
        
        sim->array_coders[i].dongle_sinistro = &sim->array_dongles[i];
        sim->array_coders[i].dongle_destro = &sim->array_dongles[(i + 1) % sim->num_coders];
        i++;
    }
    
    return (0);
}