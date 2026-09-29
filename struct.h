#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>

struct dati
{
    int num_coders;

    int tempo_burnout;
    int tempo_debug;
    int tempo_comp;
    int tempo_refactoring;

    int num_compilazioni;

    pthread_mutex_t *dongle_sinistro;
    pthread_mutex_t *dongle_destro;
};




