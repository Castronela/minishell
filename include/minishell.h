/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dstinghe <dstinghe@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/02 00:22:58 by pamatya           #+#    #+#             */
/*   Updated: 2025/01/06 18:32:29 by dstinghe         ###   ########.fr       */
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
# include <signal.h>
# include <termios.h>
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
# define BG_RESET "\033[0m"

# define C_RED "\033[31m"
# define C_GREEN "\033[32m"



//--------------------------------------------------------------------------------------//
//                              Recognized Meta Characters                              //
//--------------------------------------------------------------------------------------//

# define NEWLINE '\n'
# define SPACE ' '
# define SQ '\''
# define DQ '\"'

# define PS2 "> "
# define MV_CURSOR_BACK_PREV_LINE "\033[A"
# define MV_CURSOR_RIGHT "\033[2C"

// ---- Redirection Operators ------------------------------------------------------------

# define RD_IN "<"				// input redirection
# define RD_OUT ">"				// output redirection
# define RD_OUT_A ">>"			// append output redirection
# define RD_HD "<<"				// heredoc redirection

# define REDIRECTION_OPERATORS RD_IN, RD_OUT, RD_OUT_A, RD_HD

// ---- Control Operators ----------------------------------------------------------------

# define CT_PIPE "|"			// pipe control

# define CONTROL_OPERATORS CT_PIPE

// ---- Command Separators ---------------------------------------------------------------

# define CS_SMICOL ";"
# define CS_NEWLNE "\n"

# define COMMAND_SEPARATORS CS_SMICOL, CS_NEWLNE

// ---- Special Parameters ---------------------------------------------------------------

# define DOLLAR "$"
# define QUESTION_MARK "?"
# define POUND "#"

# define SPECIAL_OPERATORS DOLLAR, QUESTION_MARK, POUND

//--------------------------------------------------------------------------------------//
//                                    Error Messages                                    //
//--------------------------------------------------------------------------------------//

# define ERSHL C_RED "minishell: " BG_RESET

// ---- Function Error Message -----------------------------------------------------------

# define ERRMSG_MALLOC "Error malloc"
# define ERRMSG_PIPE "Error pipe"
# define ERRMSG_SIGNAL "Error signal"
# define ERRMSG_FORK "Error fork"
# define ERRMSG_READ "Error read"
# define ERRMSG_WRITE "Error write"
# define ERRMSG_OPEN "Error open"
# define ERRMSG_EXECVE "Error execve"
# define ERRMSG_WAITPID "Error waitpid"
# define ERRMSG_DUP2 "Error dup2"

// ---- Built-in Error Message -----------------------------------------------------------

# define ERRMSG_CD "cd:"

// ---- Syntax Error Message -------------------------------------------------------------

# define SYNTX_UNEXP_EOF "syntax error: unexpected end of file"
# define SYNTX_UNEXP_EOF_MATCH "unexpected EOF while looking for matching `"
# define SYNTX_UNEXP_TOKEN "syntax error near unexpected token `"

//--------------------------------------------------------------------------------------//
//                                     Error Codes                                      //
//--------------------------------------------------------------------------------------//

# define ERRCODE_SYNTAX 258


//--------------------------------------------------------------------------------------//
//                                   Type Definitions                                   //
//--------------------------------------------------------------------------------------//

typedef struct s_lst_str
{
	char				*key;		// Field introduced to replace str field, which will store the whole variable and value in case of shl->env, but will store these variables as key-value pair in separate fields for shl->variables.
	char				*val;		// This field will store the values of the variables whose names are stored by key. In case of shl->env and shl->env_paths, this field should be defaulted to NULL. This separation is done for easy extraction of variables as well as storage of other variables as users make them in the minishell.
    struct s_lst_str	*prev;
    struct s_lst_str	*next;
}   t_lst_str;

typedef struct s_cmds
{
	int				cmd_index;		// Field indicate the index of the external command; should default to 0, but tagged from 1 for first command and so on...
	int				exc_index;		// Field indicate the index of the external command; should default to 0 if command is built-in
	char			*bin_path;		// Should be constructed by looking for valid path and combining with the command call
	char			**args;			// Double char pointer to the whole command call including command its flags and its args
	t_lst_str		*heredocs_lst;	// If << is present, this will contain the heredoc string, else NULL
	int				fd_in;			// Defaults to STDINFILENO
	int				fd_out;			// Defaults to STDOUTFILENO
	int				apend;			// If >> is present, this will be set to 1, else 0
	char			*file_in;		// Name of infile if < is present, else NULL
	int				toggle_heredoc;
	char			*file_out;		// Name of outfile if > is present, else NULL
	char			*cmd_separator;	// Control operator (specifies interaction between current and succeeding command)
	struct s_cmds	*next;
}	t_cmds;

typedef struct s_shell
{
	char		**env_str;			// Copy of **envp, as required by execve fn
	t_lst_str	*env;				// Stores env variables from the calling shell
	t_lst_str	*variables;			// Stores a backup of the env variables from the calling shell
	t_lst_str	*env_paths;			// Stores the PATH variable from the calling shell
	int			shlvl;				// Stores the current shell level
	char		*cur_wd;			// Stores the current working directory
	char		*prompt;			// Stores the prompt string for the minishell

	//	Cmd vars; will be reset on every new command prompt
	pid_t		*pid;				// This stores the pid of all the processes forked during execution
	char		*cmdline;			// Stores the command line input from the user
	char		open_qt;			// Stores any existing open quote or 0 if none exist or all quotes are closed
	t_cmds		*cmds_lst;			// Stores all commands and their systemetized info about related pipes and redirections, all parsed from the command line input
	int			exit_code_prev;		// Stores the exit code from the last executed command
	int			exit_code;			// Stores the exit code from current command
}	t_shell;






//--------------------------------------------------------------------------------------//
//                                 Function Prototypes                                  //
//--------------------------------------------------------------------------------------//


int			main(int ac, char **av, char **envp);

/* ============================== src_exe/... ============================== */
/* ----------------------------- start_shell.c ----------------------------- */

void		start_shell(t_shell *shl);

/* ------------- src_exe/built_ins.c and src_exe/built_ins/.c ------------- */

int			is_built_in(char *cmd);
void		exec_built_in(t_shell *shl, t_cmds *cmd);

void		mini_echo(t_cmds *cmd);
int			mini_export(t_shell *shl, t_cmds *cmd);
void		add_str_to_double_ptr(t_shell *shl, t_cmds *cmd);
int			mini_pwd(t_shell *shl, t_cmds *cmd);
int			mini_unset(t_shell *shl, t_cmds *cmd);

void		mini_cd(t_shell *shl, t_cmds *cmd);
int			path_is_dir(char *path);
void		update_cwd(t_shell *shl, char *new_cwd);

void		mini_env(t_shell *shl, t_cmds *cmd);
void		update_last_var(t_shell *shl, t_cmds *cmd);
int			update_var_str(char *var_pointer, char *var_name, char *new_val);
int			count_pointers(char **dp);

void		mini_exit(t_shell *shl);

/* -------------------------- src_exe/init_shell.c -------------------------- */

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
void		ft_remove_node(t_lst_str **root, t_lst_str *node);
t_lst_str	*ft_find_node(t_lst_str *list, char *str, int searchfield, int mod);

/* ----------------------------- start_shell.c ----------------------------- */

int			get_binaries(t_shell *shl);
char		*get_binary_path(t_shell *shl, t_cmds *cmd);
int			remove_path(t_cmds *cmd);

/* ----------------------------- start_shell.c ----------------------------- */

void		start_shell(t_shell *shl);
void		mini_execute(t_shell *shl);
void		create_pids(t_shell *shl);
void		exec_external(t_shell *shl, t_cmds *cmd, int p_index);
void		index_cmds(t_shell *shl);
int			get_total_cmds(t_shell *shl, int which);

/* ------------------------------ redirection.c ------------------------------ */

int			open_file_fds(t_cmds *cmd);
int			set_redirections(t_cmds *cmd);
void		close_fds(t_cmds *cmd);

/* ------------------------------ utilities.c ------------------------------ */

int			compare_strings(const char *str, const char *field, int exact);
void		store_variable(t_shell *shl, char *str);
void		arg_error(char **av);
void		ft_free2dint(int **memory);
void		exit_early(t_shell *shl, char **double_ptr, char *msg);
void		clearout(t_shell *shl);
void		ft_print_lst(t_lst_str *root);

/* ============================= src_parse/... ============================= */

int			parser(t_shell *shell);
int			init_cmd_lst(t_shell *shell, t_cmds *new_cmdnode, size_t *index_cmd);

/* ------------------------------- Tokenizer ------------------------------- */

int			get_next_token(t_shell *shell, size_t *index_cmd, char **token);

/* -------------------------- Secondary prompting -------------------------- */

int			secondary_prompt(t_shell *shell, const bool prepend_nl);
int			prep_prompt(t_shell *shell, int (*hd_pipe)[2], const bool append_nl);
char		*prompt_read(t_shell *shell, int fd_read);

/* ---------------------------- Syntax Checker ---------------------------- */

bool		is_valid_quotation(t_shell *shell);
bool		is_valid_control_1(t_shell *shell);
bool		is_valid_control_2(t_shell *shell);
bool		is_redir_target_valid(t_shell *shell, const char *redir_target);

/* -------------------------------- Heredoc -------------------------------- */

int 		heredoc(t_shell *shell);

/* -------------------------- Cmds list functions -------------------------- */

t_cmds		*lst_cmds_newnode(t_shell *shell);
void		lst_cmds_addback(t_shell *shell, t_cmds *new_cmdnode);
void 		lst_cmds_freelst(t_shell *shell);

/* -------------------------- Remove closed quotes -------------------------- */

void		remove_closed_quotes(t_shell *shell, char **str);
size_t		count_closed_quotes(char *str);

/* --------------------------- Variable expansion --------------------------- */

void 		var_expansion(t_shell *shell, char **str);
void		expand_homedir_special_char(t_shell *shell, char **str);

/* ------------------------------- Pipe Setup ------------------------------- */

void 		init_pipes(t_shell *shell);

/* -------------------------------- Signals -------------------------------- */

void 		set_signal(t_shell *shell, const int handler_no);

/* --------------------------------- Utils ---------------------------------- */

bool	 	is_quote(const char c);
int			ft_fprintf_str(const int fd, const char *str[]);
int			cursor_mv_back(const int fd);
void 		skip_whitespaces(const char *str, size_t *index);
bool 		is_redir(const char *str, const size_t index);
bool 		is_control(const char *str, const size_t index);
bool 		is_special_param(const char *str, const size_t index);
bool		is_command_sep(const char *str, const size_t index);
size_t 		find_longest_match_length(const char *str, const char *pattern[]);
void		reset_cmd_vars(t_shell *shell, int free_before);
int			append_to_str(char **str, char *append, int append_len);

/* ----------------------------- Test functions ----------------------------- */

void		test_by_print(t_shell *shl);

void 		test_print_cmdlst(t_shell *shell, int spacing);
void 		test_free_cmds(t_shell *shell);
void 		test_var_exp(char **envp);
void 		test_remove_quotes(void);
void 		test_print_envariables(t_shell *shell);


/* ======================== End Function Prototypes ======================== */

#endif


// cat file
// cat path/file
// /bin/cat cat arg1 file