#include "struct.h"

void	esegui_debug(t_coder *coder)
{
	stampa_stato(coder, "is debugging");
	ft_usleep(coder->sim, coder->sim->tempo_debug);
}

void	esegui_refactoring(t_coder *coder)
{
	stampa_stato(coder, "is refactoring");
	ft_usleep(coder->sim, coder->sim->tempo_refactoring);
}

/* un solo coder ha un solo dongle: non puo compilare e va in burnout */
static void	coder_solitario(t_coder *coder)
{
	stampa_stato(coder, "has taken a dongle");
	while (!simulazione_finita(coder->sim))
		usleep(500);
}

void	*organizza(void *arg)
{
	t_coder	*coder;

	coder = (t_coder *)arg;
	if (coder->dongle_sinistro == coder->dongle_destro)
	{
		coder_solitario(coder);
		return (NULL);
	}
	while (!simulazione_finita(coder->sim))
	{
		esegui_compilazione(coder);
		if (simulazione_finita(coder->sim))
			break ;
		esegui_debug(coder);
		if (simulazione_finita(coder->sim))
			break ;
		esegui_refactoring(coder);
	}
	return (NULL);
}
