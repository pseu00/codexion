#ifndef STRUCT_H
# define STRUCT_H

# include <pthread.h>
# include <stdio.h>
# include <stdlib.h>
# include <string.h>
# include <unistd.h>
# include <sys/time.h>

# define FIFO 0
# define EDF 1

typedef struct s_sim		t_sim;
typedef struct s_dongle		t_dongle;
typedef struct s_coder		t_coder;
typedef struct s_richiesta	t_richiesta;

/* libero: orario di quando e stato liberato (0 = mai usato) */
struct s_dongle
{
	int				id;
	int				is_locked;
	long long		libero;
	pthread_mutex_t	mutex;
	pthread_cond_t	cond;
};

/* elemento della coda di priorita (heap) */
struct s_richiesta
{
	int			id_coder;
	long long	priorita;
};

struct s_coder
{
	int			id;
	pthread_t	thread;
	int			num_compilazioni;
	long long	ultimo_avvio_compilazione;
	t_dongle	*dongle_sinistro;
	t_dongle	*dongle_destro;
	t_sim		*sim;
};

/*
** print_mutex protegge l'output,
** stato_mutex protegge sim_finita, num_compilazioni e
** ultimo_avvio_compilazione (letti dal monitor),
** queue_mutex protegge la coda di attesa (heap).
*/
struct s_sim
{
	int				num_coders;
	long long		tempo_burnout;
	long long		tempo_comp;
	long long		tempo_debug;
	long long		tempo_refactoring;
	int				num_compilazioni;
	long long		dongle_cooldown;
	int				scheduler;
	long long		start_time;
	int				sim_finita;
	pthread_mutex_t	print_mutex;
	pthread_mutex_t	stato_mutex;
	t_coder			*array_coders;
	t_dongle		*array_dongles;
	t_richiesta		*coda_attesa;
	int				quanti_in_coda;
	long long		contatore_arrivi;
	pthread_mutex_t	queue_mutex;
	pthread_cond_t	queue_cond;
};

long long	get_time(void);
void		ft_usleep(t_sim *sim, long long time_in_ms);
int			simulazione_finita(t_sim *sim);
void		stampa_stato(t_coder *coder, char *messaggio);

int			heap_precede(t_richiesta *a, t_richiesta *b);
void		heap_sali(t_richiesta *heap, int i);
void		heap_scendi(t_richiesta *heap, int dimensione, int i);

void		entra_in_coda(t_sim *sim, t_coder *coder);
void		rimuovi_da_coda(t_sim *sim, int indice_da_rimuovere);
int			scegli_prossimo_in_coda(t_sim *sim, t_coder *coder);

long long	prova_a_prendere_dongles(t_coder *coder);
void		rilascia_dongles(t_coder *coder);

void		*organizza(void *arg);
void		esegui_compilazione(t_coder *coder);
void		esegui_debug(t_coder *coder);
void		esegui_refactoring(t_coder *coder);

void		*controllo_burnout(void *arg);

int			controllo_argomenti(int argc, char **argv, t_sim *sim);
int			allocazione_memoria(t_sim *sim);
int			inizializzazione_dati(t_sim *sim);
void		libera_memoria(t_sim *sim);

#endif
