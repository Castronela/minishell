# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: pamatya <pamatya@student.42heilbronn.de    +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: Invalid date        by                   #+#    #+#              #
#    Updated: 2025/01/20 17:12:57 by pamatya          ###   ########.fr        #
#                                                                              #
# **************************************************************************** #


# ----------------- Compiler Flags ----------------- #

CC			= 	gcc -g
# CC			= 	gcc -g -fsanitize=address
CFLAGS		= 	-Wall -Wextra -Werror
DEPFLAGS	= 	-MMD

# ----------------- Commands ----------------- #

RM			= 	rm -rf

# ----------------- Default Directories ----------------- #

# D_BIN		=	bin
D_INC		=	include
D_LIB		=	lib
D_OBJ		=	obj

# ----------------- Docker files ----------------- #

DK_FILE		=	Dockerfile
DK_COMP		=	docker-compose.yml

# Define the script name
DOCK_BUILD_SCRIPT = dock_build.sh

# Define the Docker image name
IMAGE_NAME = valgrind

# ----------------- Source, Object and Dependency files ----------------- #

SRC_MAIN	=	main.c test_fn.c
# SRC_MAIN	=	test_fn.c
SRC_PARSE	= 	parser_1.c parser_2.c \
				tokenizer.c \
				secondary_prompt.c \
				syntax_validation.c \
				heredoc.c \
				expansions.c \
				remove_quotes.c \
				pipes.c \
				cmds_lst_utils.c \
				signal.c \
				utils_1.c utils_2.c utils_3.c utils_4.c
SRC_EXE		=	initiate.c \
				initializations.c \
				indexing.c \
				binaries.c \
				executions.c \
				redirections.c \
				lst_str_fns.c lst_str_fns_2.c \
				bi_cd.c \
				bi_echo.c \
				bi_env.c \
				bi_exit.c \
				bi_export.c \
				bi_pwd.c \
				bi_unset.c \
				env_utils.c env_utils_2.c string_utils.c \
				error_handlers.c \
				tests.c

VPATH		+=	src_parse src_exe src_exe/built-ins

SRC			=	$(SRC_MAIN) $(SRC_PARSE) $(SRC_EXE)
OBJ 		+= 	$(addprefix $(D_OBJ)/, $(SRC:.c=.o))
DEP			= 	$(OBJ:.o=.d)

# ----------------- Target Binary ----------------- #

NAME		=	minishell

# ----------------- Headers Flag ----------------- #

HEADS_FLG	=	-I$(D_INC) -I$(LIBFT_HED) -I$(READLINE_HED)

# ----------------- Libraries ----------------- #

LIBS_FLG		=	$(LIBFT_FLG) $(READLINE_FLG)

# LIBFT
LIBFT			=	libft.a
LIBFT_DIR		=	$(D_LIB)															# library file directory
LIBFT_HED		=	$(D_LIB)/includes													# header directory
LIBFT_FLG		=	-L$(LIBFT_DIR) -l$(basename $(subst lib,,$(LIBFT)))					# library flag

# Readline for Linux
READLINE		=	libreadline.a
ifeq ($(USER), pranujamatya)
	READLINE_DIR	=	/opt/homebrew/opt/readline/lib
	READLINE_HED	=	/opt/homebrew/opt/readline/include
else
	READLINE_DIR	=	/usr/local/lib
	READLINE_HED	=	/usr/local/include
endif
READLINE_FLG	=	-L$(READLINE_DIR) -l$(basename $(subst lib,,$(READLINE)))

VALGRIND_OPTS	= 	--suppressions=ignore_readline.supp \
					--leak-check=full \
					--show-leak-kinds=all \
					--trace-children=yes \
					--track-fds=yes \
					-s \
					# --child-silent-after-fork=yes \
# Leak Sanitizer for leak check on Mac (run: make LEAK=1)
LEAKSAN			=	liblsan.dylib
LEAKSAN_DIR		=	/Users/dstinghe/LeakSanitizer
LEAKSAN_HED		= 	/Users/dstinghe/LeakSanitizer/include
LEAKSAN_FLG		=	-L$(LEAKSAN_DIR) -l$(basename $(subst lib,,$(LEAKSAN)))
		
ifeq ($(LEAK), 1)
	LIBS_FLG	+=	$(LEAKSAN_FLG)
endif


# ----------------- Color Codes ----------------- #

RESET 		= 	\033[0m
RED 		= 	\033[31m
GREEN 		= 	\033[32m
YELLOW 		= 	\033[33m
BLUE 		= 	\033[34m

# ----------------- Rules ----------------- #

all: $(NAME)
	@echo "$(GREEN)Compilation finished$(RESET)"

valgrind: $(NAME)
	env -i valgrind $(VALGRIND_OPTS) ./$(NAME)
# valgrind $(VALGRIND_OPTS) ./$(D_BIN)/$(NAME)

# dock: $(DK_COMP)
# 	@if [ $(shell docker images | grep -c valgrind) -eq 0 ]; then \
#         echo "Building valgrind image..."; \
#         docker-compose build -q; \
#     fi
# 	@docker-compose run --rm -build valgrind

# Target to execute the dock_build.sh script and run the Docker container
dock:
	@docker-compose run --rm $(IMAGE_NAME)
# @echo "Executing dock_build.sh script..."
# @./$(DOCK_BUILD_SCRIPT)
# @echo "Running Docker container..."

$(NAME): $(OBJ)
	@echo "Compiling minishell..."
	@make -sC $(D_LIB)
	@$(CC) $(CFLAGS) $(HEADS_FLG) $^ $(LIBS_FLG) -o $@ 

$(D_OBJ)/%.o: %.c
	@$(CC) $(CFLAGS) $(HEADS_FLG) $(DEPFLAGS) -c $< -o $@

-include $(DEP)

clean:
	@$(RM) $(OBJ) $(DEP)
	@make clean -sC $(D_LIB)
	@echo "$(YELLOW)Cleaned Objects & Dependencies$(RESET)"

fclean:
	@$(RM) $(OBJ) $(DEP) $(NAME)
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
