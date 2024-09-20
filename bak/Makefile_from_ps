# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: pamatya <pamatya@student.42heilbronn.de    +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2024/04/11 14:30:19 by pamatya           #+#    #+#              #
#    Updated: 2024/07/21 01:57:11 by pamatya          ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

CC		=	cc
CFLAGS	=	-Wall -Wextra -Werror

RM		=	rm -f

NAME	=	push_swap
P_LIB	=	push_swap.a
DIR		=	lib
LIB		=	libft.a
EXE_PATH=	.

SRCS	=	./src/main.c ./src/parsing.c ./src/error_checks.c ./src/list_fns.c \
			./src/op_push.c ./src/op_swap.c ./src/op_rotate.c \
			./src/op_rev_rotate.c ./src/index.c ./src/sorting.c \
			./src/algorithm.c ./src/stack_info.c ./src/utils.c

OBJS	=	$(SRCS:.c=.o)

all: $(NAME)

$(NAME): $(OBJS) $(LIB)
	cp ./$(DIR)/$(LIB) ./$(P_LIB)
	ar rcs $(P_LIB) $(OBJS)
	$(CC) $(CFLAGS) $(P_LIB) -o $(EXE_PATH)/$(NAME)

%.o: %.c
	$(CC) $(CFLAGS) -c $< -o $@

$(LIB):
	$(MAKE) -sC $(DIR) all

clean:
	$(RM) $(OBJS)
	$(MAKE) -sC $(DIR) clean

fclean: clean
	$(RM) $(EXE_PATH)/$(P_LIB)
	$(RM) $(EXE_PATH)/$(NAME)
	$(MAKE) -sC $(DIR) fclean

re: fclean all

.PHONY: all clean fclean re
