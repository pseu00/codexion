#include "struct.h"

/* a viene prima di b: priorita minore, a parita id minore */
int	heap_precede(t_richiesta *a, t_richiesta *b)
{
	if (a->priorita != b->priorita)
		return (a->priorita < b->priorita);
	return (a->id_coder < b->id_coder);
}

static void	heap_scambia(t_richiesta *a, t_richiesta *b)
{
	t_richiesta	tmp;

	tmp = *a;
	*a = *b;
	*b = tmp;
}

void	heap_sali(t_richiesta *heap, int i)
{
	int	padre;

	while (i > 0)
	{
		padre = (i - 1) / 2;
		if (!heap_precede(&heap[i], &heap[padre]))
			return ;
		heap_scambia(&heap[i], &heap[padre]);
		i = padre;
	}
}

void	heap_scendi(t_richiesta *heap, int dimensione, int i)
{
	int	figlio;

	while (2 * i + 1 < dimensione)
	{
		figlio = 2 * i + 1;
		if (figlio + 1 < dimensione
			&& heap_precede(&heap[figlio + 1], &heap[figlio]))
			figlio++;
		if (!heap_precede(&heap[figlio], &heap[i]))
			return ;
		heap_scambia(&heap[i], &heap[figlio]);
		i = figlio;
	}
}
