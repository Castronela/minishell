/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: david <david@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/02 00:22:58 by pamatya           #+#    #+#             */
/*   Updated: 2024/10/15 19:52:19 by david            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef MINISHELL_H
# define MINISHELL_H

//--------------------------------------------------------------------------------------//
//                                                                                      //
//                                       Includes                                       //
//                                                                                      //
//--------------------------------------------------------------------------------------//


# include "../lib/includes/libft.h"
# include <errno.h>
# include <readline/history.h>
# include <readline/readline.h>
# include <stdbool.h>
# include <stdio.h>
# include <stdlib.h>
# include <string.h>
# include <sys/wait.h>
# include <unistd.h>
# include <fcntl.h>

//--------------------------------------------------------------------------------------//
//                                                                                      //
//                                     Definitions                                      //
//                                                                                      //
//--------------------------------------------------------------------------------------//


//--------------------------------------------------------------------------------------//
//                                        Colors                                        //
//--------------------------------------------------------------------------------------//

# define BG_BLACK "\033[40m"
# define BG_RED "\033[41m"
# define BG_GREEN "\033[42m"
# define BG_YELLOW "\033[43m"
# define BG_BLUE "\033[44m"
# define BG_MAGENTA "\033[45m"
# define BG_CYAN "\033[46m"
# define BG_WHITE "\033[47m"

//--------------------------------------------------------------------------------------//
//                                Recognized Characters                                 //
//--------------------------------------------------------------------------------------//

# define SPACE ' '
# define QT "\'", "\""

// ---- Operators ------------------------------------------------------------------------

# define RD "<", ">", ">>"		// redirection
# define HD "<<"				// heredoc
# define CT "|"					// control

//--------------------------------------------------------------------------------------//
//                              Default File Access Rights                              //
//--------------------------------------------------------------------------------------//

# define DEFAULT_ACCESS 0644

//--------------------------------------------------------------------------------------//
//                                        Errors                                        //
//--------------------------------------------------------------------------------------//


// ---- Function Error -------------------------------------------------------------------

# define ERR_MALLOC "Error malloc"

// ---- Syntax Error ---------------------------------------------------------------------

# define ERR_STX_UNEXP_TOKEN "minishell: syntax error near unexpected token"
# define ERR_STX_OPEN_QT "minishell: syntax error: unclosed quotes"
# define ERR_STX_INCOMPLETE_OP "minishell: input error: incomplete control operator"


//--------------------------------------------------------------------------------------//
//                                      Data Types                                      //
//--------------------------------------------------------------------------------------//

// ---- Linked List ----------------------------------------------------------------------

typedef struct s_lst_str
{
	char				*str;
	struct s_lst_str	*prev;
	struct s_lst_str	*next;
}						t_lst_str;

// ---- Command data ---------------------------------------------------------------------

typedef struct s_cmds
{
	int					cmd_index;
	char				*bin_path;
	char				*bin;
	t_lst_str			*args;
	int					fd_in;
	int					fd_out;
	char *file_in;  // Name of infile if < is present, else NULL
	char *file_out; // Name of outfile if > is present, else NULL
	struct s_cmds		*next;
}						t_cmds;

typedef struct s_lst_cmds
{
	char				*input;
	t_lst_str			*tokens;
	t_lst_str			*redirs;
	int *redir_indices; // not sure yet if this is needed
	t_cmds				*cmds_lst;
}						t_lst_cmds;

// ---- Shell data -----------------------------------------------------------------------

typedef struct s_shell
{
	t_lst_str			*env;
	t_lst_str			*env_bak;
	t_lst_str			*env_paths;
	int					shlvl;
	char				*cur_wd;
	char				*prev_bin_path;
	char				*prompt;
	int					exit_code;

	bool hd_status;     // temporary
	char *hd_delimiter; // temporary

	char				*cmdline;
	t_lst_str			*tokenlst;
	int					total_cmds;
	int					**pipes;
	t_lst_cmds			*cmds;
}						t_shell;

//--------------------------------------------------------------------------------------//
//                                                                                      //
//                                 Function Prototypes                                  //
//                                                                                      //
//--------------------------------------------------------------------------------------//


int						main(void);

// ---- Linked List ----------------------------------------------------------------------

t_lst_str				*ft_lst_new(char *str);
t_lst_str				*ft_lst_last(t_lst_str *list);
void					ft_lst_addback(t_lst_str **root, t_lst_str *new);
int						ft_lst_size(t_lst_str *root);
void					ft_lst_free(t_lst_str **root);

// ---- Tokenizer ------------------------------------------------------------------------

int						tokenizer(t_shell *shell);

// ---- src_exe/... ----------------------------------------------------------------------

// src_exe/built_ins.c
int						is_built_in(char *cmd);
void					exec_built_in(t_shell *shl);
void					exec_echo(t_cmds *cmd);

// ---- Syntax Check ---------------------------------------------------------------------

int						check_syntax(t_shell *shell);

// ---- Redirection Check ----------------------------------------------------------------

int 					redirection_check(t_lst_str *head);

// ---- Shell Initialisation -------------------------------------------------------------

void					init_shell(t_shell *shl, char **envp);
void					copy_env(t_shell *shl, char **envp);
void					copy_env_paths(t_shell *shl, char **envp);
void					update_shlvl(t_shell *shl);
void					set_prompt(t_shell *shl, char *prefix, char *separator);
char					*assemble_prompt(char *prefix, char *cwd,
							char *separator);
void					exit_early(t_shell *shl, char **split, char *msg);

// ---- Variable Expansion ---------------------------------------------------------------

int						variable_expansion(t_shell *shell);

// ---- Utils ----------------------------------------------------------------------------

size_t					multicmp(const char *str, const char *valids[], bool fullmatch);
bool					is_heredoc(t_shell *shell);
void					rm_qt(char *token);

// ---- TESTERS --------------------------------------------------------------------------

void					print_lst(t_lst_str **head); // Prints content of linked list
int						test_heredoc(void);
int						test_tokenizer(void);
void					test_syntax_checker(void);
void					test_variable_expansion(void);
void 					test_redirection_check(void);

#endif
