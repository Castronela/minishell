# Compiler and flags
CC			= gcc -g
CFLAGS		= -Wall -Wextra -Werror -I$(DIR_INC)
GNUFLAGS	= #-L/usr/local/lib -I/usr/local/include -lreadline
DEPFLAGS	= -MMD

# Commands
RM			= rm -rf

# Source and object files
SRC			= main.c tokenizer.c util_0.c heredoc_0.c \
			input_checker.c var_repl.c test_fn.c lst_str_fns.c
OBJ 		= $(addprefix $(DIR_OBJ)/, $(SRC:.c=.o))
DEP			= $(OBJ:.o=.d)

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

$(NAME): $(OBJ)
	@make -sC $(DIR_LIBFT)
	$(CC) $(CFLAGS) $(GNUFLAGS) $(FLAG_LIBFT) $^ -o $(DIR_BIN)/$@

$(DIR_OBJ)/%.o: $(DIR_SRC)/%.c
	@$(CC) $(CFLAGS) $(DEPFLAGS) $(GNUFLAGS) -c $< -o $@

-include $(DEP)

clean:
	@$(RM) $(OBJ) $(DEP)

fclean: clean
	@make fclean -sC $(DIR_LIBFT)
	@$(RM) $(DIR_BIN)/$(NAME)

re: fclean all 

.PHONY: all clean fclean re