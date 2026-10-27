#include "struct.h"

/* usata solo se la creazione di un thread fallisce */
static void	ferma_simulazione(t_sim *sim)
{
	pthread_mutex_lock(&sim->stato_mutex);
	sim->sim_finita = 1;
	pthread_mutex_unlock(&sim->stato_mutex);
	pthread_mutex_lock(&sim->queue_mutex);
	pthread_cond_broadcast(&sim->queue_cond);
	pthread_mutex_unlock(&sim->queue_mutex);
}

static int	crea_coders(t_sim *sim)
{
	int	i;

	i = 0;
	while (i < sim->num_coders)
	{
		if (pthread_create(&sim->array_coders[i].thread, NULL, organizza,
				&sim->array_coders[i]) != 0)
			return (i);
		i++;
	}
	return (i);
}

static void	unisci_coders(t_sim *sim, int quanti)
{
	int	i;

	i = 0;
	while (i < quanti)
	{
		pthread_join(sim->array_coders[i].thread, NULL);
		i++;
	}
}

int	main(int argc, char **argv)
{
	t_sim		sim;
	pthread_t	thread_monitor;
	int			creati;
	int			errore;

	if (controllo_argomenti(argc, argv, &sim) || allocazione_memoria(&sim))
		return (1);
	inizializzazione_dati(&sim);
	errore = 0;
	creati = crea_coders(&sim);
	if (creati == sim.num_coders
		&& pthread_create(&thread_monitor, NULL, controllo_burnout, &sim) == 0)
		pthread_join(thread_monitor, NULL);
	else
	{
		fprintf(stderr, "Errore: impossibile creare un thread\n");
		ferma_simulazione(&sim);
		errore = 1;
	}
	unisci_coders(&sim, creati);
	libera_memoria(&sim);
	return (errore);
}
