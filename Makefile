# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: castronela <castronela@student.42.fr>      +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2024/04/11 14:30:19 by pamatya           #+#    #+#              #
#    Updated: 2024/09/29 17:06:06 by castronela       ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

# ----------------- Compiler Flags ----------------- #

CC			= 	gcc -g
CFLAGS		= 	-Wall -Wextra -Werror
DEPFLAGS	= 	-MMD

# ----------------- Commands ----------------- #

RM			= 	rm -rf

# ----------------- Directories ----------------- #

D_BIN		=	bin
D_INC		=	include
D_LIB		=	lib
D_OBJ		=	obj
D_SRC		=	src
D_SRC2		=	src_exe

# ----------------- Headers Flag ----------------- #

HEAD_F		=	-I$(D_INC) -I$(LIBFT_H) -I$(READLINE_H)

# ----------------- Source, Object and Dependency files ----------------- #

SRC			= 	main.c tokenizer.c util_0.c heredoc_0.c init_shell.c \
				input_checker.c var_repl.c test_fn.c lst_str_fns.c built_ins.c \
				redirection_check.c
OBJ 		= 	$(addprefix $(D_OBJ)/, $(SRC:.c=.o))
DEP			= 	$(OBJ:.o=.d)

# ----------------- Target Binary ----------------- #

NAME		=	minishell

# ----------------- Libraries ----------------- #

LIB_F		=	$(LIBFT_F) $(READLINE_F)

# LIBFT
LIBFT		=	libft.a
LIBFT_L		=	$(D_LIB)															# library location
LIBFT_H		=	$(D_LIB)/includes													# header location
LIBFT_F		=	-L$(LIBFT_L) -l$(basename $(subst lib,,$(LIBFT)))					# library flag

# Readline
READLINE	=	libreadline.a
READLINE_L	=	/usr/local/lib
READLINE_H	=	/usr/local/include
READLINE_F	=	-L$(READLINE_L) -l$(basename $(subst lib,,$(READLINE)))


# ----------------- Color Codes ----------------- #

RESET 		= 	\033[0m
RED 		= 	\033[31m
GREEN 		= 	\033[32m
YELLOW 		= 	\033[33m
BLUE 		= 	\033[34m

# ----------------- Rules ----------------- #

all: $(NAME)
	@echo "$(GREEN)Compilation finished$(RESET)"

$(NAME): $(OBJ)
	@echo "Compiling minishell..."
	@make -sC $(D_LIB)
	@$(CC) $(CFLAGS) $(HEAD_F) $^ $(LIB_F) -o $(D_BIN)/$@

$(D_OBJ)/%.o: $(D_SRC)/%.c
	@$(CC) $(CFLAGS) $(HEAD_F) $(DEPFLAGS) -c $< -o $@

-include $(DEP)

clean:
	@$(RM) $(OBJ) $(DEP)
	@echo "$(YELLOW)Cleaned Objects & Dependencies$(RESET)"

fclean:
	@$(RM) $(OBJ) $(DEP) $(D_BIN)/$(NAME)
	@make fclean -sC $(D_LIB)
	@echo "$(YELLOW)Cleaned All$(RESET)"

re: fclean all

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
