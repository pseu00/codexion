#include "struct.h"

/* chiamata con print_mutex e stato_mutex bloccati */
static int	coder_bruciato(t_sim *sim, t_coder *coder)
{
	long long	ora;

	ora = get_time();
	if (ora - coder->ultimo_avvio_compilazione <= sim->tempo_burnout)
		return (0);
	sim->sim_finita = 1;
	printf("%lld %d burned out\n", ora - sim->start_time, coder->id);
	return (1);
}

/* chiamata con stato_mutex bloccato */
static int	tutti_completati(t_sim *sim)
{
	int	i;

	i = 0;
	while (i < sim->num_coders)
	{
		if (sim->array_coders[i].num_compilazioni < sim->num_compilazioni)
			return (0);
		i++;
	}
	sim->sim_finita = 1;
	return (1);
}

static int	controlla_simulazione(t_sim *sim)
{
	int	i;
	int	fine;

	pthread_mutex_lock(&sim->print_mutex);
	pthread_mutex_lock(&sim->stato_mutex);
	fine = sim->sim_finita || tutti_completati(sim);
	i = 0;
	while (!fine && i < sim->num_coders)
	{
		fine = coder_bruciato(sim, &sim->array_coders[i]);
		i++;
	}
	pthread_mutex_unlock(&sim->stato_mutex);
	pthread_mutex_unlock(&sim->print_mutex);
	return (fine);
}

void	*controllo_burnout(void *arg)
{
	t_sim	*sim;

	sim = (t_sim *)arg;
	while (!controlla_simulazione(sim))
		usleep(1000);
	pthread_mutex_lock(&sim->queue_mutex);
	pthread_cond_broadcast(&sim->queue_cond);
	pthread_mutex_unlock(&sim->queue_mutex);
	return (NULL);
}
