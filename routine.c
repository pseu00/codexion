#include "struct.h"
#include <stdio.h>
#include <pthread.h>
#include <sys/time.h> 

void rilascia_dongles(t_coder *coder)
{
    pthread_mutex_lock(&coder->sim->queue_mutex);
    
    pthread_mutex_lock(&coder->dongle_sinistro->mutex);
    coder->dongle_sinistro->is_locked = 0;
    coder->dongle_sinistro->libero = get_time();
    pthread_mutex_unlock(&coder->dongle_sinistro->mutex);

    pthread_mutex_lock(&coder->dongle_destro->mutex);
    coder->dongle_destro->is_locked = 0;
    coder->dongle_destro->libero = get_time();
    pthread_mutex_unlock(&coder->dongle_destro->mutex);

    pthread_cond_broadcast(&coder->sim->queue_cond);
    pthread_mutex_unlock(&coder->sim->queue_mutex);
}

long long calcola_cooldown(t_coder *coder)
{
    long long ora;
    long long wait_sx;
    long long wait_dx;
    long long max_attesa;

    ora = get_time();
    wait_sx = (coder->dongle_sinistro->libero + coder->sim->dongle_cooldown) - ora;
    wait_dx = (coder->dongle_destro->libero + coder->sim->dongle_cooldown) - ora;
    max_attesa = 0;
    if (wait_sx > 0)
        max_attesa = wait_sx;
    if (wait_dx > max_attesa)
        max_attesa = wait_dx;
    return (max_attesa);
}

int prova_a_prendere_dongles(t_coder *coder, int idx_prossimo)
{
    long long attesa;

    if (idx_prossimo != -1 && coder->sim->coda_attesa[idx_prossimo] == coder->id)
    {
        if (coder->dongle_sinistro->is_locked == 0 && coder->dongle_destro->is_locked == 0)
        {
            attesa = calcola_cooldown(coder);
            if (attesa > 0)
            {
                pthread_mutex_unlock(&coder->sim->queue_mutex);
                ft_usleep(attesa);
                pthread_mutex_lock(&coder->sim->queue_mutex);
                return (0);
            }
            pthread_mutex_lock(&coder->dongle_sinistro->mutex);
            coder->dongle_sinistro->is_locked = 1;
            pthread_mutex_unlock(&coder->dongle_sinistro->mutex);
            pthread_mutex_lock(&coder->dongle_destro->mutex);
            coder->dongle_destro->is_locked = 1;
            pthread_mutex_unlock(&coder->dongle_destro->mutex);
            rimuovi_da_coda(coder->sim, idx_prossimo);
            return (1);
        }
    }
    return (0);
}

void esegui_compilazione(t_coder *coder)
{
    entra_in_coda(coder->sim, coder->id);
    pthread_mutex_lock(&coder->sim->queue_mutex);
    while (1)
    {
        if (coder->sim->sim_finita)
        {
            pthread_mutex_unlock(&coder->sim->queue_mutex);
            return ;
        }
        if (prova_a_prendere_dongles(coder, scegli_prossimo_in_coda(coder->sim)) == 1)
            break ;
        pthread_cond_wait(&coder->sim->queue_cond, &coder->sim->queue_mutex);
    }
    pthread_mutex_unlock(&coder->sim->queue_mutex);
    pthread_mutex_lock(&coder->sim->print_mutex);
    if (coder->sim->sim_finita == 0)
    {
        printf("%lld %d has taken a dongle\n", get_time(), coder->id);
        printf("%lld %d has taken a dongle\n", get_time(), coder->id);
        printf("%lld %d is compiling\n", get_time(), coder->id);
    }
    pthread_mutex_unlock(&coder->sim->print_mutex);
    coder->ultimo_avvio_compilazione = get_time();
    ft_usleep(coder->sim->tempo_comp);
    rilascia_dongles(coder);
}


void esegui_debug(t_coder *coder)
{
    if (coder->sim->sim_finita)
        return;
    
    pthread_mutex_lock(&coder->sim->print_mutex);
    if (!coder->sim->sim_finita)
        printf("%lld %d is debugging\n", get_time(), coder->id);
    pthread_mutex_unlock(&coder->sim->print_mutex);
    
    ft_usleep(coder->sim->tempo_debug);
}

void esegui_refactoring(t_coder *coder)
{
    if (coder->sim->sim_finita)
        return;
        
    pthread_mutex_lock(&coder->sim->print_mutex);
    if (!coder->sim->sim_finita)
        printf("%lld %d is refactoring\n", get_time(), coder->id);
    pthread_mutex_unlock(&coder->sim->print_mutex);
    
    ft_usleep(coder->sim->tempo_refactoring);
}



void esegui_cooldown(t_coder *coder)
{
    ft_usleep(coder->sim->dongle_cooldown * 1000);
}

void *organizza(void *arg)
{
    t_coder *coder = (t_coder *)arg;
    int i = 0;

    while (coder->sim->sim_finita == 0)
    {
        esegui_compilazione(coder);
        if (coder->sim->sim_finita)
            break;
            
        esegui_debug(coder);
        if (coder->sim->sim_finita)
            break;
            
        esegui_refactoring(coder);

        i++;
        if (coder->sim->num_compilazioni != -1 && i >= coder->sim->num_compilazioni)
            break;
    }
    return (NULL);
}

