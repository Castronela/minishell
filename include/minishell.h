/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pamatya <pamatya@student.42heilbronn.de    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/02 00:22:58 by pamatya           #+#    #+#             */
/*   Updated: 2025/01/17 22:53:49 by pamatya          ###   ########.fr       */
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

# define PATH_TMP_DIR "/tmp/"
# define HD_TMP_FILE_NAME ".minishell_heredoc_"
# define HD_TMP_FILE_EXT ".tmp"

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
# define UNDERSCORE "_"

// ---- Redirection Operators ------------------------------------------------------------

# define RD_IN "<"				// input redirection
# define RD_OUT ">"				// output redirection
# define RD_OUT_A ">>"			// append output redirection
# define RD_HD "<<"				// heredoc redirection

// ---- Control Operators ----------------------------------------------------------------

# define CT_PIPE "|"			// pipe control
# define CT_AND "&&"			// pipe control

// ---- Command Separators ---------------------------------------------------------------

# define CS_SMICOL ";"
# define CS_NEWLNE "\n"

// ---- Special Parameters ---------------------------------------------------------------

# define DOLLAR "$"
# define QUESTION_MARK "?"
# define POUND "#"

// ---- Reserved Bash Characters ---------------------------------------------------------

# define BT '`'
# define BN '!'
# define DL '$'
# define AD '&'
# define SC ';'
# define PO '('
# define PC ')'
# define BS '\\'
# define PP '|'

//--------------------------------------------------------------------------------------//
//                                    Error Messages                                    //
//--------------------------------------------------------------------------------------//

# define ERSHL "\033[31mminishell: \033[0m"

// ---- Function Error Message -----------------------------------------------------------

# define ERRMSG_MALLOC "Error malloc"
# define ERRMSG_PIPE "Error pipe"
# define ERRMSG_SIGNAL "Error signal"
# define ERRMSG_FORK "Error fork"
# define ERRMSG_READ "Error read"
# define ERRMSG_WRITE "Error write"
# define ERRMSG_OPEN "Error open"
# define ERRMSG_CLOSE "Error close"
# define ERRMSG_EXECVE "Error execve"
# define ERRMSG_WAITPID "Error waitpid"
# define ERRMSG_DUP "Error dup"
# define ERRMSG_DUP2 "Error dup2"
# define ERRMSG_UNLINK "Error unlink"

// ---- Heredoc Error Messages -----------------------------------------------------------

# define HD_ACCESS_DIR_TMP_FILE "cannot access directory for here-document temp\
file"
# define HD_CREATE_TMP_FILE "cannot create temp file for here-document"

// ---- Built-in Error Message -----------------------------------------------------------

# define ERRMSG_CD "cd:"

// ---- External Error Message -----------------------------------------------------------

# define ERRMSG_CMD_NOT_FOUND ": command not found\n"

// ---- Syntax Error Message -------------------------------------------------------------

# define SYNTX_UNEXP_EOF "syntax error: unexpected end of file"
# define SYNTX_UNEXP_EOF_MATCH "unexpected EOF while looking for matching `"
# define SYNTX_UNEXP_TOKEN "syntax error near unexpected token `"

//--------------------------------------------------------------------------------------//
//                                     Error Codes                                      //
//--------------------------------------------------------------------------------------//

# define ERRCODE_GENERAL 1          // General error
# define ERRCODE_BUILT_IN 2         // Error in a built-in command
# define ERRCODE_CMD_CNOT_EXEC 126  // Command found but is not executable
# define ERRCODE_CMD_NOT_FOUND 127  // Command not found

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
	int				cmd_index;		// Field to indicate the index of the external command; should default to 0, but tagged from 1 for first command and so on...
	int				exc_index;		// Field to indicate the index of the external command; should default to 0 if command is built-in
	int				lvar_assignment;// Field to indicate that this is a local variable assignment
	int				skip;
	char			*bin_path;		// Should be constructed by looking for valid path and combining with the command call
	char			**args;			// Double char pointer to the whole command call including command its flags and its args
	int				arg_count;
	// t_lst_str		*heredocs_lst;	// to be deleted
	int				fd_in;			// Defaults to STDINFILENO
	int				fd_out;			// Defaults to STDOUTFILENO
	// int				apend;			// to be deleted
	// char			*file_in;		// to be deleted
	// int				toggle_heredoc;
	// char			*file_out;		// to be deleted
	t_lst_str		*redirs;
	char			*cmd_separator;	// Control operator (specifies interaction between current and succeeding command)
	int				fd_cls;
	pid_t			pid;
	int				exit_code;
	struct s_cmds	*next;
	struct s_cmds	*prev;
}	t_cmds;

typedef struct s_shell
{
	int			ac;
	char		**av;
	int			stdio[2];
	char		**environ;			// Copy of **envp, as required by execve fn
	t_lst_str	*variables;			// Stores a backup of the env variables from the calling shell
	// t_lst_str	*env_paths;			// Borrowed pointer that points to path variable in shl->variables->val
	t_lst_str	*local_vars;		// Stores only local variables
	int			shlvl;				// Stores the current shell level
	char		*cur_wd;			// Stores the current working directory
	char		*prompt;			// Stores the prompt string for the minishell

	//	Cmd vars; will be reset on every new command prompt
	// pid_t		*pid;				// This stores the pid of all the processes forked during execution
	int			total_cmds;			// Stores the total number of commands received from a command-line input
	char		*cmdline;			// Stores the command line input from the user
	char		open_qt;			// Stores any existing open quote or 0 if none exist or all quotes are closed
	t_cmds		*cmds_lst;			// Stores all commands and their systemetized info about related pipes and redirections, all parsed from the command line input
	int			heredoc_file_no;
	int			tmp_file_fd;
	int			exit_code_prev;		// Stores the exit code from the last executed command
	int			exit_code;			// Stores the exit code from current command
}	t_shell;


//--------------------------------------------------------------------------------------//
//                                 Function Prototypes                                  //
//--------------------------------------------------------------------------------------//

int			main(int ac, char **av, char **envp);

/* ============================== src_exe/... ============================== */

/* ------------------------------- initiate.c ------------------------------- */

void		start_shell(t_shell *shl);
void 		init_shell(t_shell *shl, int ac, char **av, char **envp);
void		clearout(t_shell *shl);;

/* ------------------------------ initialize.c ------------------------------ */

void		init_environ_variables(t_shell *shl, char **envp);
void		update_shlvl(t_shell *shl);
void		set_prompt(t_shell *shl, char *prefix, char *separator);

/* ------------------------------- indexing.c ------------------------------- */

void		index_cmds(t_shell *shl);
int			is_command(t_cmds *cmd);
int			is_built_in(t_cmds *cmds);

/* ------------------------------ executions.c ------------------------------ */

void		mini_execute(t_shell *shl);
int			exec_var_assignments(t_shell *shl, t_cmds *cmd);
void		exec_built_in(t_shell *shl, t_cmds *cmd);
void		exec_pipeline(t_shell *shl, t_cmds *cmd);

/* ----------------------------- redirections.c ----------------------------- */

int 		open_file_fds(t_shell *shl, t_cmds *cmd, t_lst_str *node);
int			set_redirections(t_shell *shl, t_cmds *cmd);
void		ft_close_cmd_pipe(t_shell *shl, t_cmds *cmd, int mod);
void		ft_close_stdcpy(t_shell *shl, int mod);
int			ft_close(int fd);

/* ------------------------------- binaries.c ------------------------------- */

// int			get_binaries(t_shell *shl);
int			get_binary(t_shell *shl, t_cmds *cmd);
int			is_path(const char *str);

/* ------------------------------ exec_utils.c ------------------------------ */

// void		create_pids(t_shell *shl);
// int			get_total_cmds(t_shell *shl, int which);
void		restore_std_fds(t_shell *shl);

/* ------------- src_exe/built_ins.c and src_exe/built_ins/.c ------------- */

void		mini_cd(t_shell *shl, t_cmds *cmd);
void		mini_echo(t_cmds *cmd);
void		mini_env(t_shell *shl, t_cmds *cmd);
void		mini_exit(t_shell *shl);
int			mini_export(t_shell *shl, t_cmds *cmd);
int			mini_pwd(t_shell *shl, t_cmds *cmd);
int			mini_unset(t_shell *shl, t_cmds *cmd);

/* ------------------------------ env_utils.c ------------------------------ */

void		update_env_var(t_shell *shl, t_cmds *cmd, char *var_name, char *val);
void		store_as_variable(t_shell *shl, char *var);
void		store_local_variable(t_shell *shl, char *var);
void		add_to_environ(t_shell *shl, char *var);
char		*get_var_component(t_shell *shl, char *arg, int what);

/* ----------------------------- stirng_utils.c ----------------------------- */

int			compare_strings(const char *str, const char *field, int exact);
char		**find_string_addr(t_shell *shl, char *str, int	n);
int			find_dptr_index(t_shell *shl, char *str, int n);
int			update_environ(char **var_ptr_addr, char *var_name, char *new_val);
int			count_pointers(char **dp);
size_t		offset_to_env_value(char *str);
int			is_bash_reserved(char c);

/* ----------------------------- lst_str_fns.c ----------------------------- */

t_lst_str	*ft_lst_new(char *key, char *val);
t_lst_str	*ft_lst_last(t_lst_str *list);
void		ft_lst_addback(t_lst_str **root, t_lst_str *new);
int			ft_lst_size(t_lst_str *root);
void		ft_lst_free(t_lst_str **root);

/* ---------------------------- lst_str_fns_2.c ---------------------------- */

t_lst_str	*ft_find_node(t_lst_str *list, char *str, int searchfield, int mod);
void		ft_replace_node(t_shell *shl, t_lst_str **old, t_lst_str *new);
void		ft_remove_node(t_lst_str **root, t_lst_str **node);
void		ft_del_node(t_lst_str **node);

/* ---------------------------- error_handlers.c ---------------------------- */

void		arg_error(char **av);
void		exit_early(t_shell *shl, char **double_ptr, char *msg);

/* ============================= src_parse/... ============================= */

int			parser(t_shell *shell);
int			init_cmd_lst(t_shell *shell, t_cmds *new_cmdnode, 
			size_t *index_cmd);

/* ------------------------------- Tokenizer ------------------------------- */

int			get_next_token(t_shell *shell, size_t *index_cmd, char **token);

/* -------------------------- Secondary prompting -------------------------- */

int			secondary_prompt(t_shell *shell, const bool prepend_nl);
int			prep_prompt(t_shell *shell, int (*hd_pipe)[2], 
			const bool append_nl);
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
void		init_cmd_pipe(t_shell *shell, t_cmds *cmd);

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
void		reset_cmd_vars(t_shell *shell, const int rm_tmp);
int 		open_hd_tmp_file(t_shell *shell, t_lst_str *node);
int			append_to_str(char **str, char *append, int append_len);

/* ----------------------------- Test functions ----------------------------- */

void		test_by_print(t_shell *shl);
void		ft_print_lst(t_lst_str *root);
void 		test_print_cmdlst(t_shell *shell, int spacing);
void		test_print_1cmd(t_shell *shell, t_cmds *cmd_node, int spacing);
void 		test_free_cmds(t_shell *shell);
void 		test_var_exp(char **envp);
void 		test_remove_quotes(void);
void 		test_print_envariables(t_shell *shell);

/* ======================== End Function Prototypes ======================== */

#endif
