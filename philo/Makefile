NAME			= philo

# Compiler
CC				= cc
CFLAGS			= -Wall -Wextra -Werror 


CFLAGS			+= -fsanitize=thread


MKDIR			= mkdir -p
RM				= rm -rf
LINKER  	    = -lpthread

# Includes
INCLUDES_DIR 	= includes

INCLUDES_FLAG 	= -I$(INCLUDES_DIR)
INCLUDES		= $(INCLUDES_DIR)/philo.h

# Sources
SRCS_DIR		= srcs/

SRC_FILES		= main.c \
				  utils.c \
				  simulation.c \
				  simulation_utils.c

# Objects
OBJS_DIR		= objs/

OBJS			= $(patsubst %.c, $(OBJS_DIR)%.o, $(SRC_FILES))

# Rules
all : $(OBJS_DIR) $(NAME)

$(OBJS_DIR) :
	@$(MKDIR) $(OBJS_DIR)

$(NAME) : $(OBJS) Makefile
	$(CC) $(CFLAGS) $(OBJS) $(LINKER) -o $(NAME)

$(OBJS_DIR)%.o : $(SRCS_DIR)%.c $(INCLUDES)
	$(CC) $(CFLAGS) $(INCLUDES_FLAG) -c $< -o $@	

clean :
	$(RM) $(OBJS_DIR)

fclean : clean
	$(RM) $(NAME)

re: fclean all

.PHONY: all clean fclean re
