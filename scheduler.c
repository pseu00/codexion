#include "struct.h"

void entra_in_coda(t_sim *sim, int id_coder)
{
    pthread_mutex_lock(&sim->queue_mutex);
    sim->coda_attesa[sim->quanti_in_coda] = id_coder;
    sim->quanti_in_coda++;
    pthread_mutex_unlock(&sim->queue_mutex);
}

void rimuovi_da_coda(t_sim *sim, int indice_da_rimuovere)
{
    int i;

    i = indice_da_rimuovere;
    while (i < sim->quanti_in_coda - 1)
    {
        sim->coda_attesa[i] = sim->coda_attesa[i + 1];
        i++;
    }
    sim->quanti_in_coda--;
}


int scegli_prossimo_in_coda(t_sim *sim)
{
    int i;
    int indice_scelto;
    long long peggior_deadline;
    long long deadline_attuale;

    if (sim->quanti_in_coda == 0)
        return (-1);

    if (sim->scheduler == 0) // FIFO
        return (0);

    // EDF: Earliest Deadline First
    indice_scelto = 0;
    peggior_deadline = sim->array_coders[sim->coda_attesa[0] - 1].ultimo_avvio_compilazione + sim->tempo_burnout;
    
    i = 1;
    while (i < sim->quanti_in_coda)
    {
        deadline_attuale = sim->array_coders[sim->coda_attesa[i] - 1].ultimo_avvio_compilazione + sim->tempo_burnout;
        
        if (deadline_attuale < peggior_deadline)
        {
            peggior_deadline = deadline_attuale;
            indice_scelto = i;
        }
        else if (deadline_attuale == peggior_deadline)
        {
            if (sim->array_coders[sim->coda_attesa[i] - 1].id < sim->array_coders[sim->coda_attesa[indice_scelto] - 1].id)
                indice_scelto = i;
        }
        i++;
    }
    return (indice_scelto);
}