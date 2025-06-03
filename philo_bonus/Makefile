NAME			= philo_bonus

# Compiler
CC				= cc
CFLAGS			= -Wall -Wextra -Werror
MKDIR			= mkdir -p
RM				= rm -rf
LINKER  	    = -lpthread

# Includes
INCLUDES_DIR 	= includes

INCLUDES_FLAG 	= -I$(INCLUDES_DIR)
INCLUDES		= $(INCLUDES_DIR)/philo_bonus.h

# Sources
SRCS_DIR		= srcs/

SRC_FILES		= main_bonus.c \
				  utils_bonus.c \
				  ft_atol_bonus.c \
				  simulation_bonus.c \
				  monitor_utils_bonus.c

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
