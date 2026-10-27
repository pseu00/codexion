#include "struct.h"

/*
** Da chiamare con queue_mutex bloccato.
** FIFO: priorita = ordine di arrivo.
** EDF: priorita = ultimo_avvio_compilazione + tempo_burnout.
*/
void	entra_in_coda(t_sim *sim, t_coder *coder)
{
	t_richiesta	*richiesta;

	richiesta = &sim->coda_attesa[sim->quanti_in_coda];
	richiesta->id_coder = coder->id;
	if (sim->scheduler == EDF)
	{
		pthread_mutex_lock(&sim->stato_mutex);
		richiesta->priorita = coder->ultimo_avvio_compilazione
			+ sim->tempo_burnout;
		pthread_mutex_unlock(&sim->stato_mutex);
	}
	else
		richiesta->priorita = sim->contatore_arrivi;
	sim->contatore_arrivi++;
	sim->quanti_in_coda++;
	heap_sali(sim->coda_attesa, sim->quanti_in_coda - 1);
}

void	rimuovi_da_coda(t_sim *sim, int indice_da_rimuovere)
{
	sim->quanti_in_coda--;
	if (indice_da_rimuovere == sim->quanti_in_coda)
		return ;
	sim->coda_attesa[indice_da_rimuovere]
		= sim->coda_attesa[sim->quanti_in_coda];
	heap_scendi(sim->coda_attesa, sim->quanti_in_coda, indice_da_rimuovere);
	heap_sali(sim->coda_attesa, indice_da_rimuovere);
}

static int	condividono_dongle(t_coder *a, t_coder *b)
{
	return (a->dongle_sinistro == b->dongle_sinistro
		|| a->dongle_sinistro == b->dongle_destro
		|| a->dongle_destro == b->dongle_sinistro
		|| a->dongle_destro == b->dongle_destro);
}

/*
** Ritorna l'indice del coder nell'heap se nessuna richiesta con priorita
** piu alta chiede uno dei suoi dongle, altrimenti -1.
*/
int	scegli_prossimo_in_coda(t_sim *sim, t_coder *coder)
{
	int		i;
	int		mio;
	t_coder	*altro;

	mio = 0;
	while (mio < sim->quanti_in_coda
		&& sim->coda_attesa[mio].id_coder != coder->id)
		mio++;
	if (mio == sim->quanti_in_coda)
		return (-1);
	i = 0;
	while (i < sim->quanti_in_coda)
	{
		altro = &sim->array_coders[sim->coda_attesa[i].id_coder - 1];
		if (i != mio && condividono_dongle(coder, altro)
			&& heap_precede(&sim->coda_attesa[i], &sim->coda_attesa[mio]))
			return (-1);
		i++;
	}
	return (mio);
}
