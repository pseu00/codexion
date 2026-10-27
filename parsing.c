#include "struct.h"

static int	errore(char *messaggio)
{
	fprintf(stderr, "Errore: %s\n", messaggio);
	return (1);
}

/* accetta solo cifre, niente stringa vuota, niente overflow di int */
static int	converti_numero(char *str, long long *valore)
{
	int	i;

	i = 0;
	*valore = 0;
	if (str[0] == '\0')
		return (1);
	while (str[i])
	{
		if (str[i] < '0' || str[i] > '9')
			return (1);
		*valore = *valore * 10 + (str[i] - '0');
		if (*valore > 2147483647)
			return (1);
		i++;
	}
	return (0);
}

static void	salva_valori(t_sim *sim, long long *valori)
{
	sim->num_coders = (int)valori[0];
	sim->tempo_burnout = valori[1];
	sim->tempo_comp = valori[2];
	sim->tempo_debug = valori[3];
	sim->tempo_refactoring = valori[4];
	sim->num_compilazioni = (int)valori[5];
	sim->dongle_cooldown = valori[6];
}

int	controllo_argomenti(int argc, char **argv, t_sim *sim)
{
	long long	valori[7];
	int			i;

	if (argc != 9)
		return (errore("servono esattamente 8 argomenti"));
	i = 0;
	while (i < 7)
	{
		if (converti_numero(argv[i + 1], &valori[i]))
			return (errore("gli argomenti devono essere interi >= 0"));
		i++;
	}
	if (valori[0] <= 0)
		return (errore("serve almeno un coder"));
	if (strcmp(argv[8], "fifo") == 0)
		sim->scheduler = FIFO;
	else if (strcmp(argv[8], "edf") == 0)
		sim->scheduler = EDF;
	else
		return (errore("lo scheduler deve essere fifo o edf"));
	salva_valori(sim, valori);
	return (0);
}
