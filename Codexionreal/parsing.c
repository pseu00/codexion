#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "struct.h"

int controllo_argomenti(int argc, char **argv)
{
    if (argc != 9)
    {
        printf("Errore: \n");
        return 1;
    }
        

    int i;
    int a;
    i = 1;
    
    while (i <= 7)
    {
        a = 0;
        while(argv[i][a])
        {
            if(!(argv[i][a] >= '0' && argv[i][a] <= '9'))
            {
                printf("Errore");
                return 1;
            }
            a++;
        }
        i++;
    }

    if (strcmp(argv[8], "fifo") != 0 && strcmp(argv[8], "edf") != 0) 
    {
        printf("Errore:\n");
        return 1;
    }


    t_sim sim;

    sim.num_coders = atoi(argv[1]);
    sim.tempo_burnout = atoi(argv[2]);
    sim.tempo_comp = atoi(argv[3]);
    sim.tempo_debug = atoi(argv[4]);
    sim.tempo_refactoring = atoi(argv[5]);
    sim.num_compilazioni = atoi(argv[6]);
    sim.dongle_cooldown = atoi(argv[7]);
    strcpy(sim.scheduler, argv[8]);

    pthread_t thread_test;
    int id_test = 1;

    
    if ((sim.num_coders <= 0) || (sim.tempo_burnout < 0) || (sim.tempo_comp < 0) || (sim.tempo_debug < 0) || (sim.tempo_refactoring < 0) || (sim.num_compilazioni < 0))
    {
        printf("Errore:\n");
        return 1;
    }

}

