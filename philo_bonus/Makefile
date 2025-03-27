NAME = philo_bonus

SRC = main.c actions.c utils.c

$(NAME): $(SRC) philosophers.h
	cc -Wall -Wextra -Werror -pthread -o $(NAME) $(SRC)

all: $(NAME)

clean:
	
fclean:
	rm -f $(NAME)

re: fclean all

.PHONY: all clean fclean re bonus