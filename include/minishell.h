#ifndef MINISHELL_H
# define MINISHELL_H

# include "../lib/Libft/include/libft.h"
# include <errno.h>
# include <stdbool.h>
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

/*
Structural changes:
- t_shell_data is now just t_shell
- tlsttoken now also has a prev pointer
*/

/*--- Type Definitions ---*/

typedef struct e_lsttoken
{
	char				*token;
	struct e_lsttoken	*prev;
	struct e_lsttoken	*next;
}	t_lsttoken;

typedef struct s_cmd
{
	char			*cmd;
	char			**args;
	int				fd_in;
	int				fd_out;
	int				file_in;
	int				file_out;
	struct s_cmd	*prev;
	struct s_cmd	*next;
}	t_cmd;

typedef struct s_pipes_redirs
{
	int	*pipe_fds;
	int	*redir_fds;
	int	*redir_flags;
	int	*redir_files;
	int	*redir_files_flags;
	int	*redir_files_fds;
}	t_pipes_redirs;

typedef struct s_cmds_lst
{
	char			*input;
	char			*token;
	int				total_cmds;
	t_pipes_redirs	*pr;
	t_cmd			*cmd;
}	t_cmds_lst;

typedef struct s_shell
{
	char		**envp;
	t_lsttoken	**tokenlst;
	t_cmds_lst	*cmds_lst;
	char		*cmdline;
	bool		hd_status;
	char		*hd_delimiter;
}	t_shell;

/* Function Prototypes */
int			main(void);

/*--- Token linked list ---*/
t_lsttoken	**tokenlst_memalloc(void);
int			tokenlst_addtoken(t_lsttoken **head, char *token);
void		tokenlst_memfreelist(t_lsttoken **head);

/*--- Tokenizer ---*/
int			tokenizer(t_shell *shell);
char		*get_next_token(t_shell *shell, ssize_t *i_cmdline);

/*--- Syntax Checker ---*/
bool		is_quoteclosed(t_lsttoken *node_current);
bool		is_op_invalid(const char *op);
int			check_op(t_lsttoken *node_current, t_lsttoken *node_prev);
int			check_syntax(t_shell *shell);

/*--- Heredoc ---*/
bool		is_heredoc(t_shell *shell);
void		rm_qt(char *token);

/*--- Utils ---*/
bool		is_ws(const char c);
bool		is_op(const char c);
bool		is_qt(const char c);

/*--- Testing ---*/
void		print_lst(t_lsttoken **head);
int			test_heredoc(void);
int			test_tokenizer(void);
void 		test_syntax_checker(void);

/* End Function Prototypes */

#endif
