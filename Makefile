# Nome dell'eseguibile finale
NAME = codexion

# Compilatore e Flags
CC = cc
CFLAGS = -Wall -Wextra -Werror -pthread

# Rimuovi file
RM = rm -f

# File Sorgente (Aggiungi qui tutti i tuoi file .c)
SRCS = main.c

# File Oggetto (Generati automaticamente dai sorgenti)
OBJS = $(SRCS:.c=.o)

# Regola principale
all: $(NAME)

# Regola di compilazione per l'eseguibile (il collegamento)
$(NAME): $(OBJS)
	$(CC) $(CFLAGS) $(OBJS) -o $(NAME)

# Regola per compilare i singoli file .c in .o
%.o: %.c
	$(CC) $(CFLAGS) -c $< -o $@

# Regola per pulire i file oggetto (.o)
clean:
	$(RM) $(OBJS)

# Regola per pulire tutto (oggetti ed eseguibile)
fclean: clean
	$(RM) $(NAME)

# Regola per pulire e ricompilare tutto da zero
re: fclean all

# Indica a 'make' che queste regole non sono nomi di file
.PHONY: all clean fclean re                                                                                                                                                                                                                                                                                        