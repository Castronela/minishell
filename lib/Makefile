# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: pamatya <pamatya@student.42heilbronn.de    +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2024/04/11 14:30:19 by pamatya           #+#    #+#              #
#    Updated: 2024/07/04 04:08:55 by pamatya          ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

CC		=	cc
CFLAGS	=	-Wall -Wextra -Werror
RM		=	rm -f

NAME	=	libft.a

A1		=	./Libft/ft_is
A2		=	./Libft/ft_list
A3		=	./Libft/ft_memory
A4		=	./Libft/ft_put_fd
A5		=	./Libft/ft_string
A6		=	./Libft/ft_to
B		=	./ft_printf
B1		=	./ft_printf/Utils
C		=	./GNL

SRCS	=	$(A1)/ft_isalnum.c $(A1)/ft_isalpha.c $(A1)/ft_isascii.c $(A1)/ft_isdigit.c $(A1)/ft_isprint.c \
			$(A2)/ft_lstadd_back.c $(A2)/ft_lstadd_front.c $(A2)/ft_lstclear.c $(A2)/ft_lstdelone.c $(A2)/ft_lstiter.c $(A2)/ft_lstlast.c $(A2)/ft_lstmap.c $(A2)/ft_lstnew.c $(A2)/ft_lstsize.c \
			$(A3)/ft_bzero.c $(A3)/ft_memchr.c $(A3)/ft_memcmp.c $(A3)/ft_memcpy.c $(A3)/ft_memmove.c $(A3)/ft_memset.c $(A3)/ft_calloc.c $(A3)/ft_realloc.c $(A3)/ft_free.c \
			$(A4)/ft_putchar_fd.c $(A4)/ft_putendl_fd.c $(A4)/ft_putnbr_fd.c $(A4)/ft_putstr_fd.c \
			$(A5)/ft_split.c $(A5)/ft_strchr.c $(A5)/ft_strdup.c $(A5)/ft_striteri.c $(A5)/ft_strjoin.c $(A5)/ft_strlcat.c $(A5)/ft_strlcpy.c $(A5)/ft_strlen.c $(A5)/ft_strmapi.c $(A5)/ft_strncmp.c $(A5)/ft_strnstr.c $(A5)/ft_strrchr.c $(A5)/ft_strtrim.c $(A5)/ft_substr.c \
			$(A6)/ft_atoi.c $(A6)/ft_itoa.c $(A6)/ft_tolower.c $(A6)/ft_toupper.c \
			$(B)/ft_printf.c $(B1)/ft_form.c $(B1)/ft_putdec.c $(B1)/ft_puthex.c $(B1)/ft_putptr.c $(B1)/ft_putstr.c $(B1)/ft_utils.c $(B1)/ft_lengths.c \
			$(C)/get_next_line.c
			
OBJS	=	$(SRCS:.c=.o)

all: $(NAME)

$(NAME): $(OBJS)
	ar rcs $(NAME) $(OBJS)

%.o: %.c
	$(CC) $(CFLAGS) -c $< -o $@

clean:
	$(RM) $(OBJS)

fclean: clean
	$(RM) $(NAME)

re: fclean all

.PHONY: all clean fclean re
