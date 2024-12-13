/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pamatya <pamatya@student.42heilbronn.de    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/02 00:22:58 by pamatya           #+#    #+#             */
/*   Updated: 2024/12/13 17:14:02 by pamatya          ###   ########.fr       */
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



/*
Structural changes:
- t_shell_data is now just t_shell
...
*/

/*  -------------- Type Definitions -------------- */

//Standard error message definitions here...
/* -------------------------------- STD_ERR -------------------------------- */
# define ERR_STX_QT "minishell: syntax error: unclosed quotes"
# define ERR_STX_OP "minishell: syntax error near unexpected token"
# define ERR_MALLOC "minishell: malloc failed"


/* -------------------------------- STD_ERR -------------------------------- */

typedef struct s_lst_str
{
    char				*str;
    struct s_lst_str	*prev;
    struct s_lst_str	*next;
}   t_lst_str;

typedef struct s_cmds
{
	int				cmd_index;	// Don't know if we require this, yet
	char			*bin_path;		// Should be constructed by looking for valid path and combining with the command call
	// char			*bin;			// Maybe this is not necessary
	char			**args;			// Double char pointer to the whole command call including command its flags and its args
	char			*hd_str;		// If << is present, this will contain the heredoc string, else NULL
	int				fd_in;			// Defaults to STDINFILENO
	int				fd_out;			// Defaults to STDOUTFILENO
	int				apend;			// If >> is present, this will be set to 1, else 0
	char			*file_in;		// Name of infile if < is present, else NULL
	char			*file_out;		// Name of outfile if > is present, else NULL
	struct s_cmds	*next;
}	t_cmds;

typedef struct s_shell
{
	t_lst_str	*env;				// Stores env variables from the calling shell
	t_lst_str	*env_bak;			// Stores a backup of the env variables from the calling shell
	t_lst_str	*env_paths;			// Stores the PATH variable from the calling shell
	int			shlvl;				// Stores the current shell level
	char		*cur_wd;			// Stores the current working directory
	char		*last_bin_arg;		// Stores the last argument of the binary that was last executed
	char		*prompt;			// Stores the prompt string for the minishell

	char		*cmdline;			// Stores the command line input from the user
	t_cmds		*cmds_lst;			// Stores all commands and their systemetized info about related pipes and redirections, all parsed from the command line input
	int			exit_code;			// Stores the exit code from the last executed command
}	t_shell;

/*  ========================== Function Prototypes ========================== */

// src/main.c
int			main(int ac, char **av, char **envp);
// int			main(int ac, char **av);


/* ------------------------------ src_exe/... ------------------------------ */
// src_exe/built_ins.c
int		is_built_in(char *cmd);
void	exe_built_in(t_shell *shl);
void	exe_echo(t_cmds *cmd);
int		compare_strings(const char *str1, const char *str2, int abs_toggle);

// src_exe/init_shell.c
void 	init_shell(t_shell *shl, char **envp);
void	copy_env(t_shell *shl, char **envp);
void	copy_env_paths(t_shell *shl, char **envp);
void	update_shlvl(t_shell *shl);
void	set_prompt(t_shell *shl, char *prefix, char *separator);
char	*assemble_prompt(char *prefix, char *cwd, char *separator);

void	exit_early(t_shell *shl, char **split, char *msg);
void	ft_print_lst(t_lst_str *root);


/* lst_str_fns.c */
t_lst_str	*ft_lst_new(char *str);
t_lst_str	*ft_lst_last(t_lst_str *list);
void		ft_lst_addback(t_lst_str **root, t_lst_str *new);
int			ft_lst_size(t_lst_str *root);
void		ft_lst_free(t_lst_str **root);



/* ------------------------------ src_parse/... ------------------------------ */
/*--- Tokenizer ---*/


/*--- Syntax Checker ---*/


/*--- Heredoc ---*/


/*--- Utils ---*/


/*--- Testing ---*/


/* End Function Prototypes */

#endif
