#ifndef STRUCT_H
#define STRUCT_H


#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>

typedef struct s_sim t_sim;
typedef struct s_dongle t_dongle;
typedef struct s_coder t_coder;
 
struct s_dongle
{
    int id;
    int is_locked;          
    long long libero;  //orario di quando e stato liberato
    pthread_mutex_t mutex; // lucketto per non modificare i dati nello stesso momento
    pthread_cond_t cond;   //condizone di attesa

};


struct s_coder 
{
    int id;
    pthread_t thread;
    int num_compilazioni; // quante compilazioni fin ora
    long long ultimo_avvio_compilazione; // orario di quando inizia a compilare

    t_dongle *dongle_sinistro;   
    t_dongle *dongle_destro;

    t_sim *sim;                 
};

struct s_sim  
{
    int num_coders;
    long long tempo_burnout;   //tempo senza ndere i dongle prima di morire
    long long tempo_comp;      // tempo che ci mette a comp. (trattiene i dongle)
    long long tempo_debug;        //Azioni secondarie vhe fa funa volta finito di compilare
    long long tempo_refactoring;
    int num_compilazioni;
    long long dongle_cooldown; //devi aspettare tot prima di provare a riprenderli
    int scheduler;        // fifo o edf

    int sim_finita;       //per controllare se fermare il ciclo
    pthread_mutex_t print_mutex; //lucchetto per quando stampare

    struct s_coder *array_coders; 
    t_dongle *array_dongles;


    int             *coda_attesa;
    int             quanti_in_coda;
    pthread_mutex_t queue_mutex;
    pthread_cond_t  queue_cond;  

};

long long get_time(void);
void ft_usleep(long long time_in_ms);

void entra_in_coda(t_sim *sim, int id_coder);
void rimuovi_da_coda(t_sim *sim, int indice_da_rimuovere);
int scegli_prossimo_in_coda(t_sim *sim);

void *organizza(void *arg);
void esegui_compilazione(t_coder *coder);
void esegui_debug(t_coder *coder);
void esegui_refactoring(t_coder *coder);

void *controllo_burnout(void *arg);

int controllo_argomenti(int argc, char **argv, t_sim *sim);
int allocazione_memoria(t_sim *sim);
int inizializzazione_dati(t_sim *sim);

#endif

