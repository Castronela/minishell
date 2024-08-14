#ifndef MINISHELL_H
# define MINISHELL_H

# include "../lib/Libft/include/libft.h"
# include <errno.h>
# include <stdbool.h>
# include <stdio.h>
# include <stdlib.h>
# include <string.h>
# include <sys/wait.h>
# include <unistd.h>

# define BG_BLACK "\033[40m"
# define BG_RED "\033[41m"
# define BG_GREEN "\033[42m"
# define BG_YELLOW "\033[43m"
# define BG_BLUE "\033[44m"
# define BG_MAGENTA "\033[45m"
# define BG_CYAN "\033[46m"
# define BG_WHITE "\033[47m"

/*--- Type Definitions ---*/

typedef struct e_lsttoken
{
	char				*token;
	struct e_lsttoken	*next;
}						t_lsttoken;

typedef struct s_shell_data
{
	t_lsttoken			**tokenlst;
	char				*cmdline;
}						t_shell_data;

/* Function Prototypes */

int						main(void);

/*--- Tokenizer ---*/

int						tokenizer(t_shell_data *shell);
char					*get_next_token(const char *cmdline,
							ssize_t *i_cmdline);

/*--- Token linked list*/

t_lsttoken				**tokenlst_memalloc(void);
int						tokenlst_addtoken(t_lsttoken **head, char *token);
void					tokenlst_memfreelist(t_lsttoken **head);

/*--- Utils ---*/

bool					is_whitespace(const char c);
bool					is_operator(const char c);
bool					is_quote(const char c);

/*--- Testing ---*/

void					print_lst(t_lsttoken **head);
int						tokenizer_tester(void);

/* End Function Prototypes */

#endif
