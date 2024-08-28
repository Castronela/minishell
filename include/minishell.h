# ifndef MINISHELL_H
# define MINISHELL_H

# include "../lib/Libft/include/libft.h"
# include <stdio.h>
# include <stdlib.h>
# include <readline/readline.h>
# include <readline/history.h>

typedef struct s_inputs
{
    char            *input;
    struct s_inputs *prev;
    struct s_inputs *next;
}   t_inputs;


# endif 
