/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pamatya <pamatya@student.42heilbronn.de    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/02 00:22:58 by pamatya           #+#    #+#             */
/*   Updated: 2025/02/02 20:36:14 by pamatya          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef MINISHELL_H
# define MINISHELL_H

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

//----------------------------------------------------------------------------//
//                          Recognized Meta Characters                        //
//----------------------------------------------------------------------------//

# define NEWLINE '\n'
# define SPACE ' '
# define SQ '\''
# define DQ '\"'

# define PS2 "> "
# define MV_CURSOR_BACK_PREV_LINE "\033[A"
# define MV_CURSOR_RIGHT "\033[2C"
# define UNDERSCORE "_"

// ---- Redirection Operators --------------------------------------------------

# define RD_IN "<"				// input redirection
# define RD_OUT ">"				// output redirection
# define RD_OUT_A ">>"			// append output redirection
# define RD_HD "<<"				// heredoc redirection

// ---- Control Operators ------------------------------------------------------

# define CT_PIPE "|"			// pipe control
# define CT_AND "&&"			// pipe control

// ---- Special Parameters -----------------------------------------------------

# define DOLLAR "$"
# define QUESTION_MARK "?"
# define POUND "#"

// ---- Reserved Bash Characters -----------------------------------------------

# define BT '`'
# define BN '!'
# define DL '$'
# define AD '&'
# define SC ';'
# define PO '('
# define PC ')'
# define BS '\\'
# define PP '|'

//----------------------------------------------------------------------------//
//                                Error Messages                              //
//----------------------------------------------------------------------------//

# define ERSHL "\033[31mminishell: \033[0m"

// ---- Function Error Message -------------------------------------------------

# define ERRMSG_MALLOC "Error malloc"
# define ERRMSG_PIPE "Error pipe"
# define ERRMSG_SIGNAL "Error signal"
# define ERRMSG_FORK "Error fork"
# define ERRMSG_READ "Error read"
# define ERRMSG_WRITE "Error write"
# define ERRMSG_PRINTF "Error printf"
# define ERRMSG_OPEN "Error open"
# define ERRMSG_CLOSE "Error close"
# define ERRMSG_EXECVE "Error execve"
# define ERRMSG_WAITPID "Error waitpid"
# define ERRMSG_DUP "Error dup"
# define ERRMSG_DUP2 "Error dup2"
# define ERRMSG_UNLINK "Error unlink"
# define ERRMSG_TCGETATTR "Error tcgetattr"
# define ERRMSG_TCSETATTR "Error tcsetattr"

// ---- Heredoc Error Messages -------------------------------------------------

# define HD_ACCESS_DIR_TMP_FILE "cannot access directory for here-document temp\
file"
# define HD_CREATE_TMP_FILE "cannot create temp file for here-document"

// ---- Built-in Error Message -------------------------------------------------

# define ERRMSG_CD "cd: "
# define ERRMSG_NO_VALID_IDENT "\': not a valid identifier\n"
# define ERRMSG_PATH_IS_DIR ": Is a directory\n"

// ---- External Error Message -------------------------------------------------

# define ERRMSG_CMD_NOT_FOUND ": command not found\n"

// ---- Syntax Error Message ---------------------------------------------------

# define SYNTX_UNEXP_EOF "syntax error: unexpected end of file"
# define SYNTX_UNEXP_EOF_MATCH "unexpected EOF while looking for matching `"
# define SYNTX_UNEXP_TOKEN "syntax error near unexpected token `"

//----------------------------------------------------------------------------//
//                                Error Codes                                 //
//----------------------------------------------------------------------------//

# define ERRCODE_GENERAL 1
# define ERRCODE_BUILT_IN 2
# define ERRCODE_CMD_CNOT_EXEC 126
# define ERRCODE_CMD_OR_FILE_NOT_FOUND 127

//----------------------------------------------------------------------------//
//                              Type Definitions                              //
//----------------------------------------------------------------------------//

typedef struct s_lst_str
{
	char				*key;
	char				*val;
	struct s_lst_str	*prev;
	struct s_lst_str	*next;
}	t_lst_str;

typedef struct s_cmds
{
	int				cmd_index;
	int				exc_index;
	int				lvar_assignment;
	int				skip;
	char			*bin_path;
	char			**args;
	int				arg_count;
	int				fd_in;
	int				fd_out;
	t_lst_str		*redirs;
	char			*cmd_separator;
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
	char		**environ;
	char		**env_paths;
	t_lst_str	*variables;
	t_lst_str	*local_vars;
	int			shlvl;
	char		*home_dir;
	char		*owd;
	char		*cwd;
	char		*prompt;

	int			total_cmds;
	char		*prev_cmdline;
	char		*cmdline;
	char		open_qt;
	t_cmds		*cmds_lst;
	int			heredoc_file_no;
	int			tmp_file_fd;
	int			exit_code_prev;
	int			exit_code;
}	t_shell;

//----------------------------------------------------------------------------//
//                            Function Prototypes                             //
//----------------------------------------------------------------------------//

int			main(int ac, char **av, char **envp);

/* =============================== src_exe/... ============================== */

/* ------------------------------- initiate.c ------------------------------- */

void		init_shell(t_shell *shl, int ac, char **av, char **envp);
void		start_shell(t_shell *shl);
char		*get_input(t_shell *shell, const char *prompt);
void		clearout(t_shell *shl);

/* --------------------------- initializations.c ---------------------------- */

void		init_environ_variables(t_shell *shl, char **envp);
void		update_shlvl(t_shell *shl);
void		set_prompt(t_shell *shl, char *prefix, char *separator);

/* ------------------------------- indexing.c ------------------------------- */

void		index_cmds(t_shell *shl);
int			is_command(t_cmds *cmd);
int			is_built_in(t_cmds *cmds);

/* ------------------------------ executions.c ------------------------------ */

void		mini_execute(t_shell *shl);
void		exec_built_in(t_shell *shl, t_cmds *cmd);
void		exec_pipeline(t_shell *shl, t_cmds *cmd);
void		exec_var_assignments(t_shell *shl, t_cmds *cmd);

/* ------------------------------- child_fns.c ------------------------------ */

void		handle_child_exit(t_shell *shl, t_cmds *cmd);
void		exec_child(t_shell *shl, t_cmds *cmd);

/* ----------------------------- redirections.c ----------------------------- */

int			set_redirs(t_shell *shl, t_cmds *cmd);
int			dup_std_fds(t_shell *shl, t_cmds *cmd);
void		ft_close_cmd_pipe(t_shell *shl, t_cmds *cmd, int mod);
int			ft_close(int fd);

/* ------------------------------- binaries.c ------------------------------- */

void		set_binaries(t_shell *shl, t_cmds *cmd);
void		set_env_paths(t_shell *shl);
int			is_path(const t_cmds *cmd);

/* -------------- src_exe/built_ins.c and src_exe/built_ins/.c -------------- */

void		mini_cd(t_shell *shl, t_cmds *cmd);
void		mini_echo(t_cmds *cmd);
void		mini_env(t_shell *shl, t_cmds *cmd);
void		mini_exit(t_shell *shl, t_cmds *cmd);
int			mini_export(t_shell *shl, t_cmds *cmd);
int			mini_pwd(t_shell *shl, t_cmds *cmd);
int			mini_unset(t_shell *shl, t_cmds *cmd);

/* ------------------------------- env_utils.c ------------------------------ */

int			update_environ(char **var_ptr_addr, char *var_name, char *new_val);
char		*get_var_component(t_shell *shl, char *arg, int what);
void		update_env_var(t_shell *shl, t_cmds *cmd, char *var_name,
				char *val);

/* ----------------------- variables and environment.c ---------------------- */

void		add_to_environ(t_shell *shl, char *var, int append);
void		store_as_variable(t_shell *shl, char *var, int append);
void		store_local_variable(t_shell *shl, char *var, int append);

/* ----------------------------- stirng_utils*.c ---------------------------- */

int			compare_strings(const char *str, const char *field, int exact);
char		**find_string_addr(t_shell *shl, char *str, int n, int add_eq);
int			find_dptr_index(t_shell *shl, char *str, int n);
int			count_pointers(char **dp);
size_t		var_offset(char *str, int skip_separator);
char		*concat_strings(const char *str[]);

/* ----------------------------- lst_str_fns*.c ----------------------------- */

t_lst_str	*ft_lst_new(char *key, char *val);
t_lst_str	*ft_lst_last(t_lst_str *list);
void		ft_lst_addback(t_lst_str **root, t_lst_str *new);
int			ft_lst_size(t_lst_str *root);
void		ft_lst_free(t_lst_str **root);
t_lst_str	*ft_find_node(t_lst_str *list, char *str, int searchfield, int mod);
void		ft_replace_node(t_shell *shl, t_lst_str **old, t_lst_str *new);
void		ft_remove_node(t_lst_str **root, t_lst_str **node);
void		ft_del_node(t_lst_str **node);

/* -------------------------------- utils_4.c ------------------------------- */

void		restore_std_fds(t_shell *shl);
int			path_is_dir(char *path);
int			is_valid_name(char *arg, int *i);
int			get_append_flag(int check);

/* ---------------------------- error_handlers.c ---------------------------- */

void		arg_error(char **av);
void		exit_early(t_shell *shl, char **double_ptr, char *msg);
void		exit_early2(t_shell *shl, char **double_ptr, char *s_ptr,
				char *msg);

/* ============================= src_parse/... ============================= */

int			parser(t_shell *shell);
int			init_cmd_lst(t_shell *shell, t_cmds *new_cmdnode,
				size_t *index_cmd);
void		retokenize_args(t_shell *shell, t_cmds *cmd);
void		map_args(t_shell *shell, t_cmds *cmd,
				void (*func)(t_shell *, char **));

/* ------------------------------- Tokenizer ------------------------------- */

int			get_next_token(t_shell *shell, size_t *index_cmd, char **token,
				const int do_complex);

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

int			heredoc(t_shell *shell);

/* -------------------------- Cmds list functions -------------------------- */

t_cmds		*lst_cmds_newnode(t_shell *shell);
void		lst_cmds_addback(t_shell *shell, t_cmds *new_cmdnode);
void		lst_cmds_freelst(t_shell *shell);

/* -------------------------- Remove closed quotes -------------------------- */

void		remove_closed_quotes(t_shell *shell, char **str);
size_t		count_closed_quotes(char *str);

/* --------------------------- Variable expansion --------------------------- */

void		var_expansion(t_shell *shell, char **str);
void		expand_homedir_special_char(t_shell *shell, char **str);

/* ------------------------------- Pipe Setup ------------------------------- */

void		init_cmd_pipe(t_shell *shell, t_cmds *cmd);

/* -------------------------------- Signals -------------------------------- */

void		set_signal(t_shell *shell);
void		tty_echo_sig(t_shell *shell, const bool echo);

/* --------------------------------- Utils ---------------------------------- */

bool		is_quote(const char c);
int			ft_fprintf_str(const int fd, const char *str[]);
int			cursor_mv_back(const int fd);
int			is_whitesp(const char c);
void		skip_whitespaces(const char *str, size_t *index);
bool		is_redir(const char *str, const size_t index);
bool		is_control(const char *str, const size_t index);
bool		is_special_param(const char *str, const size_t index);
size_t		find_longest_match_length(const char *str, const char *pattern[]);
void		reset_cmd_vars(t_shell *shell, const int rm_tmp,
				const int free_prev_cmdline);
int			open_hd_tmp_file(t_shell *shell, t_lst_str *node);
int			append_to_str(char **str, char *append, int append_len);

/* ----------------------------- Test functions ----------------------------- */

void		test_by_print(t_shell *shl);
void		ft_print_lst(t_lst_str *root);
void		test_print_cmdlst(t_shell *shell, int spacing);
void		test_print_1cmd(t_shell *shell, t_cmds *cmd_node, int spacing);
void		test_free_cmds(t_shell *shell);
void		test_var_exp(char **envp);
void		test_remove_quotes(void);
void		test_print_envariables(t_shell *shell);

/* ======================== End Function Prototypes ======================== */

#endif
