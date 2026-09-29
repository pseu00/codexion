#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>

int main(int argc, char **argv)
{
    if (argc != 7)
    {
        printf("Errore");
        return 1;
    }
        

    int i;
    int a;
    i = 1;
    
    while (argv[i])
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

    int num_coders = atoi(argv[1]);
    int tempo_burnout = atoi(argv[2]);
    int tempo_comp = atoi(argv[3]);
    int tempo_debug = atoi(argv[4]);
    int tempo_refactoring = atoi(argv[5]);
    int num_compilazioni = atoi(argv[6]);

    if ((num_coders <= 0) || (tempo_burnout < 0) || (tempo_comp < 0) || (tempo_debug < 0) || (tempo_refactoring < 0) || (num_compilazioni < 0))
    {
        printf("Errore");
        return 1;
    }
}