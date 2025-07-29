NAME = philo
SRC = philo.c utilities.c philo_utilities.c threads.c run_sim.c short_sleep.c init_mtx.c clean.c

CC = cc
CFLAGS = -Wextra -Wall -Werror -g3
OBJ = $(SRC:.c=.o)

all: $(NAME)

$(NAME): $(OBJ)
	$(CC)  $(OBJ) $(CFLAGS) -o $(NAME)

clean:
	rm -f $(OBJ)

fclean: clean
	make clean
	rm -f $(NAME)

re : fclean all clean

.PHONY : all clean fclean
