#include "struct.h"

long long	get_time(void)
{
	struct timeval	time;

	gettimeofday(&time, NULL);
	return ((long long)time.tv_sec * 1000 + time.tv_usec / 1000);
}

/* dorme a piccoli passi e si interrompe se la simulazione e finita */
void	ft_usleep(t_sim *sim, long long time_in_ms)
{
	long long	start;

	start = get_time();
	while ((get_time() - start) < time_in_ms)
	{
		if (simulazione_finita(sim))
			return ;
		usleep(500);
	}
}

int	simulazione_finita(t_sim *sim)
{
	int	finita;

	pthread_mutex_lock(&sim->stato_mutex);
	finita = sim->sim_finita;
	pthread_mutex_unlock(&sim->stato_mutex);
	return (finita);
}

void	stampa_stato(t_coder *coder, char *messaggio)
{
	t_sim	*sim;

	sim = coder->sim;
	pthread_mutex_lock(&sim->print_mutex);
	if (!simulazione_finita(sim))
		printf("%lld %d %s\n", get_time() - sim->start_time,
			coder->id, messaggio);
	pthread_mutex_unlock(&sim->print_mutex);
}
