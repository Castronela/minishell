# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: pamatya <pamatya@student.42heilbronn.de    +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2024/04/11 14:30:19 by pamatya           #+#    #+#              #
#    Updated: 2024/09/04 22:47:51 by pamatya          ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

# Compiler and flags
CC			=	cc
CFLAGS		=	-Wall -Wextra -Werror
# CFLAGS		=	-Wall -Wextra -Werror -g -fsanitize=address

# Commands
RM			=	rm -f

# Directories
# DIR_BIN		=	.
DIR_BIN		=	bin
DIR_INC		=	include
DIR_LIB		=	lib
DIR_OBJ		=	obj
DIR_SRC		=	src
DIR_SRC2	=	src_exe

# Libraries and paths
LIBFT		=	libft.a
# LIBS		=	-lft -lreadline
# LIBS		=	$(LIBFT) -lft -lreadline
LIBS		=	-lft -lreadline
LIB_PATHS	=	-L$(DIR_LIB) -L/usr/local/lib

# Header paths
# HEAD_PATHS		=	-I ./$(DIR_INC) -I ./$(DIR_LIB)/includes
HEAD_PATHS		=	-I$(DIR_INC) -I$(DIR_LIB)/includes -I/usr/local/include

# Source and object files
# SRCS		=	$(DIR_SRC)/main.c
SRCS		=	$(DIR_SRC)/main.c \
				$(DIR_SRC2)/init_shell.c $(DIR_SRC2)/lst_str_fns.c
OBJS		=	$(SRCS:.c=.o)

# Target Binary
NAME		=	minishell
BINARY		=	$(DIR_BIN)/$(NAME)


# ----------------- Rules ----------------- #
all: $(NAME)

# $(NAME): $(addprefix $(DIR_OBJ)/, $(OBJ)) $(LIBFT)
$(NAME): $(OBJS) $(LIBFT)
	$(CC) $(CFLAGS) $(OBJS) $(HEAD_PATHS) $(LIB_PATHS) $(LIBS) -o $(BINARY)

# $(DIR_OBJ)/%.o: %.c
%.o: %.c
	$(CC) $(CFLAGS) -c $< -o $@

$(LIBFT):
	@$(MAKE) -sC $(DIR_LIB) all

clean:
	@$(RM) $(OBJS)
	@$(MAKE) -sC $(DIR_LIB) clean

fclean: clean
	@$(RM) $(BINARY)
	@$(RM) $(DIR_LIB)/libft.a
	@$(MAKE) -sC $(DIR_LIB) fclean

re: fclean
	@$(MAKE) all

bug: $(OBJS) $(LIBFT)
	$(CC) $(CFLAGS) $(OBJS) $(HEAD_PATHS) $(LIB_PATHS) $(LIBS) -o $(DIR_BIN)/bug

run: re
	@./$(DIR_BIN)/$(NAME)

val:
	@valgrind --leak-check=full ./$(DIR_BIN)/$(NAME) 
# @valgrind --leak-check=full --show-leak-kinds=all --track-origins=yes ./$(DIR_BIN)/$(NAME)
# @valgrind --leak-check=full --show-leak-kinds=all ./$(DIR_BIN)/$(NAME)
# @valgrind --leak-check=full --show-leak-kinds=all --gen-suppressions=all ./$(DIR_BIN)/$(NAME) 2>val_sup.txt
# @valgrind --leak-check=full --show-leak-kinds=all --suppressions=readline_suppressions.supp ./your_program

# valgpt: re
# 	@valgrind --leak-check=full --gen-suppressions=all --suppressions=<(
# 	cat <<EOF
# 	{
# 	readline_leak
# 	Memcheck:Leak
# 	match-leak-kinds: reachable
# 	fun:malloc
# 	fun:xmalloc
# 	fun:rl_malloc
# 	fun:_rl_init_line_state
# 	fun:readline
# 	}
# 	EOF
# 	) ./$(DIR_BIN)/$(NAME)

.PHONY: all clean fclean re
