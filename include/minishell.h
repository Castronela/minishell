/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pamatya <pamatya@student.42heilbronn.de    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/02 00:22:58 by pamatya           #+#    #+#             */
/*   Updated: 2024/09/23 14:10:26 by pamatya          ###   ########.fr       */
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
	// int				cmd_index;	// Don't know if we require this, yet
	char			*bin_path;		// Should be constructed by looking for valid path and combining with the command call
	char			*bin;			// Maybe this is not necessary
	char			**args;			// Double char pointer to the whole command call including command its flags and its args
	int				fd_in;			// Defaults to STDINFILENO
	int				fd_out;			// Defaults to STDOUTFILENO
	char			*file_in;		// Name of infile if < is present, else NULL
	char			*file_out;		// Name of outfile if > is present, else NULL
	struct s_cmds	*next;
}	t_cmds;

typedef struct s_lst_cmds
{
	char				*input;
	t_lst_str			*tokens;
	t_lst_str			*redirs;
	int					*redir_indices;	// not sure yet if this is needed
	t_cmds				*cmds_lst;
}	t_lst_cmds;

typedef struct s_shell
{
	t_lst_str	*env;
	t_lst_str	*env_bak;
	t_lst_str	*env_paths;
	int			shlvl;
	char		*cur_wd;
	char		*prev_bin_path;
	char		*prompt;
	int			exit_code;

	char		*cmdline;
	bool		hd_status;		// to be clarified: what is this for?
	char		*hd_delimiter;	// to be clarified: what is this for?

	// t_lsttoken	**tokenlst;		// if it is a list, do we need a double pointer here? Is it not easier to make this a **char since it can be easily split using ft_split?
	t_lst_str	*tokenlst;
	int			total_cmds;
	int			**pipes;
	t_lst_cmds	*cmds;
}	t_shell;


/*  ========================== Function Prototypes ========================== */

// src/main.c
int			main(int ac, char **av, char **envp);
// int			main(int ac, char **av);


/* ------------------------------ src_exe/... ------------------------------ */
// src_exe/built_ins.c
int		is_built_in(char *cmd);
void	exec_built_in(t_shell *shl);
void	exec_echo(t_cmds *cmd);


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
