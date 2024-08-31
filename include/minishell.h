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

# define ERR_STX_QT "minishell: syntax error: unclosed quotes"
# define ERR_STX_OP "minishell: syntax error near unexpected token"

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
	bool				hd_status;
	char				*hd_delimiter;
}						t_shell_data;

/* Function Prototypes */

int						main(void);

/*--- Token linked list ---*/

t_lsttoken				**tokenlst_memalloc(void);
int						tokenlst_addtoken(t_lsttoken **head, char *token);
void					tokenlst_memfreelist(t_lsttoken **head);

/*--- Tokenizer ---*/

int						tokenizer(t_shell_data *shell);
char					*get_next_token(t_shell_data *shell,
							ssize_t *i_cmdline);

/*--- Syntax Checker ---*/

bool					is_quoteclosed(t_lsttoken *node_current);
bool				    is_op_invalid(const char *op);
int						check_op(t_lsttoken *node_current, t_lsttoken *node_prev);
int						check_syntax(t_shell_data *shell);

/*--- Heredoc ---*/

bool					is_heredoc(t_shell_data *shell);
void					rm_qt(char *token);

/*--- Utils ---*/

bool					is_ws(const char c);
bool					is_op(const char c);
bool					is_qt(const char c);

/*--- Testing ---*/

void					print_lst(t_lsttoken **head);
int						test_heredoc(void);
int						test_tokenizer(void);
void 					test_syntax_checker(void);

/* End Function Prototypes */

#endif
