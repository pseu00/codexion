NAME        = codexion

CC          = cc
CFLAGS      = -Wall -Wextra -Werror -pthread
RM          = rm -f

SRCS        = main.c \
              init.c \
              parsing.c \
              routine.c \
              compilazione.c \
              dongle.c \
              monitor.c \
              utils.c \
              scheduler.c \
              heap.c

OBJS        = $(SRCS:.c=.o)

all:        $(NAME)

$(NAME):    $(OBJS)
	$(CC) $(CFLAGS) $(OBJS) -o $(NAME)

%.o:        %.c struct.h
	$(CC) $(CFLAGS) -c $< -o $@

clean:
	$(RM) $(OBJS)

fclean:     clean
	$(RM) $(NAME)

re:         fclean all

.PHONY:     all clean fclean re
