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

# define BG_BLACK 				"\033[40m"
# define BG_RED 				"\033[41m"
# define BG_GREEN 				"\033[42m"
# define BG_YELLOW 				"\033[43m"
# define BG_BLUE 				"\033[44m"
# define BG_MAGENTA 			"\033[45m"
# define BG_CYAN 				"\033[46m"
# define BG_WHITE 				"\033[47m"

# define WHITE_SPACE 			' ', 0
# define QUOTE 					'\'', '\"', 0
# define OPERATOR_REDIRECTION 	"<", "<<", ">", ">>", NULL
# define OPERATOR_CONTROL 		"|", NULL

# define ERR_MALLOC 			"Error malloc"

# define ERR_STX_UNEXP_TOKEN 	"minishell: syntax error near unexpected token"
# define ERR_STX_OPEN_QT 		"minishell: syntax error: unclosed quotes"
# define ERR_STX_INCOMPLETE_OP 	"minishell: input error: incomplete control operator"

/*--- Type Definitions ---*/

typedef struct s_lst_str
{
	char				*str;
	struct s_lst_str	*next;
}						t_lst_str;

typedef struct s_shell
{
	t_lst_str			*tokenlst;
	char				*cmdline;
	bool				hd_status;
	char				*hd_delimiter;
	size_t				token_count;
}						t_shell;

/* Function Prototypes */

int						main(void);

/*--- Token linked list ---*/

t_lst_str				*ft_lst_new(char *str);
t_lst_str				*ft_lst_last(t_lst_str *list);
void					ft_lst_addback(t_lst_str **root, t_lst_str *new);
int						ft_lst_size(t_lst_str *root);
void					ft_lst_free(t_lst_str **root);

/*--- Tokenizer ---*/

int						tokenizer(t_shell *shell);
char					*get_next_token(t_shell *shell, ssize_t *i_cmdline);
int						add_token_to_lst(t_lst_str **root, char *token);
bool					op_in_token(const char *str, int start,
							ssize_t *i_cmdline);

/*--- Syntax Checker ---*/

bool					is_quoteclosed(t_lst_str *node_current);
int						check_op(t_lst_str *node_current, t_lst_str *node_prev);
int						check_syntax(t_shell *shell);

/*--- Heredoc ---*/

bool					is_heredoc(t_shell *shell);
void					rm_qt(char *token);

/*--- Variable Expansion ---*/

int						variable_expansion(t_shell *shell);
int						subst_vars(t_lst_str *node);
char					*subst_var_value(char *token, char *variable, ssize_t *var_index);
char					*get_var(const char *token, ssize_t i);

/*--- Utils ---*/

bool					is_ws(const char c);
bool					is_qt(const char c);
bool					is_op_ctrl_char(const char *str, ssize_t *size);
bool					is_op_redir_char(const char *str, ssize_t *size);

/*--- Testing ---*/

void					print_lst(t_lst_str **head);
int						test_heredoc(void);
int						test_tokenizer(void);
void					test_syntax_checker(void);
void					test_variable_expansion(void);

/* End Function Prototypes */

#endif
