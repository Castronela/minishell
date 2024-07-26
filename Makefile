# Compiler and flags
CC			= gcc -g
CFLAGS		= -Wall -Wextra -Werror -I$(DIR_INC)

# Commands
RM			= rm -rf

# Source and object files
SRC			= main.c 
OBJ			:= $(SRC:.c=.o)

# Directories
DIR_BIN		= bin
DIR_INC		= include
DIR_LIB		= lib
DIR_OBJ		= obj
DIR_SRC		= src

# Library Libft
DIR_LIBFT	= $(DIR_LIB)/Libft
FLAG_LIBFT	= -L$(DIR_LIBFT) -lft

# output file
NAME		= minishell


all: $(NAME)

$(NAME): $(addprefix $(DIR_OBJ)/, $(OBJ))
	make -C $(DIR_LIBFT)
	$(CC) $(CFLAGS) $^ $(FLAG_LIBFT) -o $(DIR_BIN)/$@

$(DIR_OBJ)/%.o: $(DIR_SRC)/%.c 
	$(CC) $(CFLAGS) -c $< -o $@

clean:
	make clean -C $(DIR_LIBFT)
	$(RM) $(addprefix $(DIR_OBJ)/, $(OBJ))

fclean: clean
	make fclean -C $(DIR_LIBFT)
	$(RM) $(DIR_BIN)/$(NAME)

re: fclean all 

.PHONY: all clean fclean re