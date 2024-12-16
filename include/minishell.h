/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pamatya <pamatya@student.42heilbronn.de    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/02 00:22:58 by pamatya           #+#    #+#             */
/*   Updated: 2024/12/16 19:17:32 by pamatya          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef MINISHELL_H
# define MINISHELL_H

// # include "../lib/Libft/include/libft.h"
# include "../lib/includes/libft.h"
# include <errno.h>
# include <stdbool.h>
# include <stdio.h>
# include <stdlib.h>
# include <string.h>
# include <sys/wait.h>
# include <sys/stat.h>
# include <unistd.h>
# include <readline/readline.h>
# include <readline/history.h>

# define BG_BLACK "\033[40m"
# define BG_RED "\033[41m"
# define BG_GREEN "\033[42m"
# define BG_YELLOW "\033[43m"
# define BG_BLUE "\033[44m"
# define BG_MAGENTA "\033[45m"
# define BG_CYAN "\033[46m"
# define BG_WHITE "\033[47m"



//--------------------------------------------------------------------------------------//
//                              Recognized Meta Characters                              //
//--------------------------------------------------------------------------------------//

# define NEWLINE '\n'
# define SPACE ' '
# define SQ '\''
# define DQ '\"'

// ---- Redirection Operators ------------------------------------------------------------

# define RD_IN "<"				// input redirection
# define RD_OUT ">"				// output redirection
# define RD_OUT_A ">>"			// append output redirection
# define RD_HD "<<"				// heredoc redirection

# define REDIRECTION_OPERATORS RD_IN, RD_OUT, RD_OUT_A, RD_HD

// ---- Control Operators ----------------------------------------------------------------

# define CT_PIPE "|"			// pipe control

# define CONTROL_OPERATORS CT_PIPE




//--------------------------------------------------------------------------------------//
//                                    Error Messages                                    //
//--------------------------------------------------------------------------------------//

// ---- Function Error Message -----------------------------------------------------------

# define ERRMSG_MALLOC "Error malloc"

// ---- Syntax Error Message -------------------------------------------------------------

# define ERRMSG_UNEXP_TOKEN "minishell: syntax error near unexpected token"
# define ERRMSG_OPEN_QUOTE "minishell: syntax error: unclosed quotes"
# define ERRMSG_INCOMPLETE_CONTROL_OPERATOR "minishell: syntax error: incomplete control operator"




//--------------------------------------------------------------------------------------//
//                                   Type Definitions                                   //
//--------------------------------------------------------------------------------------//

typedef struct s_lst_str
{		// This later may be replaced by 'key' as its redundant. It is kept for now so as not to disrupt workflow with partner's branch/work as well as while mergin with test.
	char				*key;		// Field introduced to replace str field, which will store the whole variable and value in case of shl->env, but will store these variables as key-value pair in separate fields for shl->variables.
	char				*val;		// This field will store the values of the variables whose names are stored by key. In case of shl->env and shl->env_paths, this field should be defaulted to NULL. This separation is done for easy extraction of variables as well as storage of other variables as users make them in the minishell.
    struct s_lst_str	*prev;
    struct s_lst_str	*next;
}   t_lst_str;

typedef struct s_cmds
{
	int				cmd_index;		// Don't know if we require this, yet
	char			*bin_path;		// Should be constructed by looking for valid path and combining with the command call
	// char			*bin;			// Maybe this is not necessary
	char			**args;			// Double char pointer to the whole command call including command its flags and its args
	t_lst_str		*hd_str;		// If << is present, this will contain the heredoc string, else NULL
	int				fd_in;			// Defaults to STDINFILENO
	int				fd_out;			// Defaults to STDOUTFILENO
	int				apend;			// If >> is present, this will be set to 1, else 0
	char			*file_in;		// Name of infile if < is present, else NULL
	char			*file_out;		// Name of outfile if > is present, else NULL
	char			*ctl_operator;	// Control operator (specifies interaction between current and succeeding command)
	struct s_cmds	*next;
}	t_cmds;

typedef struct s_shell
{
	t_lst_str	*env;				// Stores env variables from the calling shell
	t_lst_str	*variables;			// Stores a backup of the env variables from the calling shell
	t_lst_str	*env_paths;			// Stores the PATH variable from the calling shell
	int			shlvl;				// Stores the current shell level
	char		*cur_wd;			// Stores the current working directory
	char		*last_bin_arg;		// Stores the last argument of the binary that was last executed
	char		*prompt;			// Stores the prompt string for the minishell
	int			exit_code;			// Stores the exit code from the last executed command

	char		*cmdline;			// Stores the command line input from the user
	char		open_qt;			// Stores any existing open quote or 0 if none exist or all quotes are closed
	t_cmds		*cmds_lst;			// Stores all commands and their systemetized info about related pipes and redirections, all parsed from the command line input
}	t_shell;






//--------------------------------------------------------------------------------------//
//                                 Function Prototypes                                  //
//--------------------------------------------------------------------------------------//


// src_parse/main.c

int			main(int ac, char **av, char **envp);
// src_exe/test_main.c
void		test_initiations(t_shell *shl, int ac, char **av, char **envp);


/* ============================== src_exe/... ============================== */

/* ------------- src_exe/built_ins.c and src_exe/built_ins/....c ------------- */

int			is_built_in(char *cmd);
void		exec_built_in(t_shell *shl, t_cmds *cmd);

void		mini_cd(t_shell *shl);
void		mini_echo(t_shell *shl);
int			mini_export(t_shell *shl, t_cmds *cmd);
int			mini_pwd(t_shell *shl, t_cmds *cmd);

/* ------------------------- src_exe/init_shell.c ------------------------- */

void 		init_shell(t_shell *shl, char **envp);
void		copy_env(t_shell *shl, char **envp);
void		copy_env_paths(t_shell *shl, char **envp);
void		update_shlvl(t_shell *shl);
void		set_prompt(t_shell *shl, char *prefix, char *separator);
char		*assemble_prompt(char *prefix, char *cwd, char *separator);

/* ----------------------------- lst_str_fns.c ----------------------------- */

t_lst_str	*ft_lst_new(char *key, char *val);
t_lst_str	*ft_lst_last(t_lst_str *list);
void		ft_lst_addback(t_lst_str **root, t_lst_str *new);
int			ft_lst_size(t_lst_str *root);
void		ft_lst_free(t_lst_str **root);
void		ft_replace_node(t_lst_str *old, t_lst_str *new);
void		ft_del_node(t_lst_str *node);
t_lst_str	*ft_find_node(t_lst_str *list, char *str, const int search_field);

/* ------------------------------ utilities.c ------------------------------ */

int			compare_strings(const char *str, const char *field, int exact);
void		store_variable(t_shell *shl, char *str);
void		arg_error(char **av);
void		exit_early(t_shell *shl, char **split, char *msg);
void		clearout(t_shell *shl);
void		ft_print_lst(t_lst_str *root);

/* ============================= src_parse/... ============================= */

void		parser(t_shell *shell);

/* --- Tokenizer --- */

char		*get_next_token(t_shell *shell, size_t *index_cmd);

/* --- Syntax Checker --- */

bool		is_valid_quotation(t_shell *shell);
bool		is_valid_control(t_shell *shell);
bool		is_redir_target_valid(char *redir_target);

/* --- Heredoc --- */


/* --- Cmds list functions --- */

t_cmds		*lst_cmds_newnode(t_shell *shell);
void		lst_cmds_addback(t_shell *shell, t_cmds *new_cmdnode);
void 		lst_cmds_freelst(t_shell *shell);

/* --- Utils --- */

bool	 	is_quote(const char c);
void 		skip_whitespaces(const char *str, size_t *index);
void 		skip_quoted_str(t_shell *shell, const char *str, size_t *index);
bool 		is_redir(const char *str, const size_t index);
bool 		is_control(const char *str, const size_t index);
size_t 		find_longest_match_length(const char *str, const char *pattern[]);

/* ----------------------------- Test functions ----------------------------- */

void 		get_normal_input(t_shell *shell);
void 		test_print_cmdlst(t_shell *shell);
void 		test_free_cmds(t_shell *shell);
void 		test_new_tokenizer(void);

/* ======================== End Function Prototypes ======================== */

#endif
