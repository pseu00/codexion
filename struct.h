
#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>


typedef struct s_tmp   
{
    int num_coders;
    long long tempo_burnout;
    long long tempo_comp;
    long long tempo_debug;
    long long tempo_refactoring;
    int num_compilazioni;
    long long dongle_cooldown;
    char *scheduler[5];        

    int sim_finita;        
    pthread_mutex_t print_mutex;

    struct s_coder *array_coders;
    struct s_dongle *array_dongles;

} t_sim;

 
typedef struct s_dongle
{
    int id;
    int is_locked;          
    long long libero;
    pthread_mutex_t mutex;
    pthread_cond_t cond;   

} t_dongle;


typedef struct s_coder 
{
    int id;
    pthread_t thread;
    int num_compilazioni;
    long long ultimo_avvio_compilazione;

    t_dongle *dongle_sinistro;   
    t_dongle *dongle_destro;

    t_sim *sim;                 
} t_coder;

