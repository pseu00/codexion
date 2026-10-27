#include "struct.h"

int	allocazione_memoria(t_sim *sim)
{
	sim->array_coders = malloc(sizeof(t_coder) * sim->num_coders);
	sim->array_dongles = malloc(sizeof(t_dongle) * sim->num_coders);
	sim->coda_attesa = malloc(sizeof(t_richiesta) * sim->num_coders);
	if (!sim->array_coders || !sim->array_dongles || !sim->coda_attesa)
	{
		free(sim->array_coders);
		free(sim->array_dongles);
		free(sim->coda_attesa);
		fprintf(stderr, "Errore: malloc fallita\n");
		return (1);
	}
	return (0);
}

static void	inizializza_dongles(t_sim *sim)
{
	int	i;

	i = 0;
	while (i < sim->num_coders)
	{
		sim->array_dongles[i].id = i;
		sim->array_dongles[i].is_locked = 0;
		sim->array_dongles[i].libero = 0;
		pthread_mutex_init(&sim->array_dongles[i].mutex, NULL);
		pthread_cond_init(&sim->array_dongles[i].cond, NULL);
		i++;
	}
}

static void	inizializza_coders(t_sim *sim)
{
	int	i;

	i = 0;
	while (i < sim->num_coders)
	{
		sim->array_coders[i].id = i + 1;
		sim->array_coders[i].num_compilazioni = 0;
		sim->array_coders[i].sim = sim;
		sim->array_coders[i].ultimo_avvio_compilazione = sim->start_time;
		sim->array_coders[i].dongle_sinistro = &sim->array_dongles[i];
		sim->array_coders[i].dongle_destro
			= &sim->array_dongles[(i + 1) % sim->num_coders];
		i++;
	}
}

int	inizializzazione_dati(t_sim *sim)
{
	pthread_mutex_init(&sim->print_mutex, NULL);
	pthread_mutex_init(&sim->stato_mutex, NULL);
	pthread_mutex_init(&sim->queue_mutex, NULL);
	pthread_cond_init(&sim->queue_cond, NULL);
	sim->sim_finita = 0;
	sim->quanti_in_coda = 0;
	sim->contatore_arrivi = 0;
	sim->start_time = get_time();
	inizializza_dongles(sim);
	inizializza_coders(sim);
	return (0);
}

void	libera_memoria(t_sim *sim)
{
	int	i;

	i = 0;
	while (i < sim->num_coders)
	{
		pthread_mutex_destroy(&sim->array_dongles[i].mutex);
		pthread_cond_destroy(&sim->array_dongles[i].cond);
		i++;
	}
	pthread_mutex_destroy(&sim->print_mutex);
	pthread_mutex_destroy(&sim->stato_mutex);
	pthread_mutex_destroy(&sim->queue_mutex);
	pthread_cond_destroy(&sim->queue_cond);
	free(sim->array_coders);
	free(sim->array_dongles);
	free(sim->coda_attesa);
}
