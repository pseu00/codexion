#include "struct.h"

static void	imposta_dongle(t_dongle *dongle, int bloccato, long long ora)
{
	pthread_mutex_lock(&dongle->mutex);
	dongle->is_locked = bloccato;
	if (!bloccato)
		dongle->libero = ora;
	pthread_mutex_unlock(&dongle->mutex);
}

/* -1 se il dongle e in uso, altrimenti i ms di cooldown rimanenti */
static long long	attesa_dongle(t_dongle *dongle, long long cooldown)
{
	long long	attesa;

	pthread_mutex_lock(&dongle->mutex);
	attesa = -1;
	if (!dongle->is_locked)
	{
		attesa = dongle->libero + cooldown - get_time();
		if (attesa < 0)
			attesa = 0;
	}
	pthread_mutex_unlock(&dongle->mutex);
	return (attesa);
}

static long long	calcola_cooldown(t_coder *coder)
{
	long long	cooldown;
	long long	wait_sx;
	long long	wait_dx;

	cooldown = coder->sim->dongle_cooldown;
	wait_sx = attesa_dongle(coder->dongle_sinistro, cooldown);
	wait_dx = attesa_dongle(coder->dongle_destro, cooldown);
	if (wait_sx < 0 || wait_dx < 0)
		return (-1);
	if (wait_sx > wait_dx)
		return (wait_sx);
	return (wait_dx);
}

/*
** Da chiamare con queue_mutex bloccato.
** 0 = dongle presi, -1 = aspettare un segnale, >0 = ms di cooldown.
*/
long long	prova_a_prendere_dongles(t_coder *coder)
{
	long long	attesa;
	int			idx;

	idx = scegli_prossimo_in_coda(coder->sim, coder);
	if (idx == -1)
		return (-1);
	attesa = calcola_cooldown(coder);
	if (attesa != 0)
		return (attesa);
	imposta_dongle(coder->dongle_sinistro, 1, 0);
	imposta_dongle(coder->dongle_destro, 1, 0);
	rimuovi_da_coda(coder->sim, idx);
	return (0);
}

void	rilascia_dongles(t_coder *coder)
{
	long long	ora;

	pthread_mutex_lock(&coder->sim->queue_mutex);
	ora = get_time();
	imposta_dongle(coder->dongle_sinistro, 0, ora);
	imposta_dongle(coder->dongle_destro, 0, ora);
	pthread_cond_broadcast(&coder->sim->queue_cond);
	pthread_mutex_unlock(&coder->sim->queue_mutex);
}
