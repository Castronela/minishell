/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   initiate.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pamatya <pamatya@student.42heilbronn.de    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/17 21:46:09 by pamatya           #+#    #+#             */
/*   Updated: 2025/01/26 15:43:27 by pamatya          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/minishell.h"

void 	init_shell(t_shell *shl, int ac, char **av, char **envp);
void	start_shell(t_shell *shl);
char 	*get_input(t_shell *shell, const char *prompt);
void	clearout(t_shell *shl);

static void setup_cmd(t_shell *shl);

// static int	skip_assignments(t_cmds *cmd);

/*
Initializes the elements of the shell struct "t_shell"
  - Copies the environment variables to the shell struct
  - Copies the PATH variable to the shell struct
  - Updates the SHLVL variable
  - Sets the current working directory
  - Sets the prompt
  - Frees all malloc's and exits the program if any of the above steps fail

!!! Need to look into update_shlvl for leaks and other potential problems
-->>	Potentially totally fixed. Haven't checked with valgrind yet.
-->>	No issues with valgrind so far
*/
void	init_shell(t_shell *shl, int ac, char **av, char **envp)
{
	*shl = (t_shell) {
		.ac = ac,
		.av = av,
		.stdio[0] = dup(STDIN_FILENO),
		.stdio[1] = dup(STDOUT_FILENO),
		.shlvl = 1,
		.tmp_file_fd = -1
	};
	if (shl->stdio[0] < 0 || shl->stdio[1] < 0)
	{
		if (shl->stdio[1] != -1)
			close(shl->stdio[1]);
		if (shl->stdio[0] != -1)
			close(shl->stdio[0]);
		exit_early(NULL, NULL, ERRMSG_DUP);
	}
	init_environ_variables(shl, envp);
	update_shlvl(shl);
	shl->cur_wd = getcwd(NULL, 0);
	if (!shl->cur_wd)
		exit_early(shl, NULL, "getcwd");
	set_prompt(shl, "<< ", " >> % ");
}

/*
Function to start shell execution

!!!	Confirm the annotation on the break condition (on the presence of break 
	statement)
*/
void	start_shell(t_shell *shl)
{	
	while (1)
	{
		set_signal(shl);
		tty_echo_sig(shl, false);
		shl->cmdline = get_input(shl, shl->prompt);
		if (!shl->cmdline)
			break;
		if (parser(shl))
		{
			if (*shl->cmdline)
				add_history(shl->cmdline);
			reset_cmd_vars(shl, 1);
			continue ;
		}
		if (*shl->cmdline)
			add_history(shl->cmdline);
		retokenize_args(shl, shl->cmds_lst);	
		index_cmds(shl);
		// test_print_cmdlst(shl, 30);
		setup_cmd(shl);
		if (shl->cmds_lst)
			mini_execute(shl);
		reset_cmd_vars(shl, 1);
	}
}

char *get_input(t_shell *shell, const char *prompt)
{
	char *input;
	char *line;
	
	if (isatty(STDIN_FILENO))
		input = readline(prompt);
	else
	{
		line = get_next_line(STDIN_FILENO);
		if (!line)
			return (NULL);
		input = ft_strtrim(line, "\n");
		free(line);
		if (!input)
			exit_early(shell, NULL, ERRMSG_MALLOC);
	}
	return (input);
}

static void setup_cmd(t_shell *shl)
{
	t_cmds *cmd;

	cmd = shl->cmds_lst;
	set_env_paths(shl);
	while (cmd)
	{
		map_args(shl, cmd, remove_closed_quotes);
		if (!set_redirs(shl, cmd))
			set_binaries(shl, cmd);
		cmd = cmd->next;
	}
}

/*
Function to free any allocated memory during the minishell session and return
  - When called, the fn frees all allocated t_shell field pointers, clears the 
	readline history, and return control to the calling function.
  - This function is meant to exit minishell without memory leaks from pointers
	accesible from the main stack.
  - The funciton will not free any memory allocated locally by the calling fn
	which is not accessible through the main stack.
  - The function also does not free any t_cmds struct nodes with the assumption
	that this will occur within the minishell session at the end of each command
	cycle.
*/
void	clearout(t_shell *shl)
{
	if (shl->environ != NULL)
		ft_free2d_safe(&shl->environ);
	if (shl->variables != NULL)
		ft_lst_free(&shl->variables);
	if (shl->local_vars != NULL)
		ft_lst_free(&shl->local_vars);
	if (shl->home_dir != NULL)
		ft_free_safe((void **)(&(shl->home_dir)));
	if (shl->old_wd != NULL)
		ft_free_safe((void **)(&(shl->old_wd)));
	if (shl->cur_wd != NULL)
		ft_free_safe((void **)(&(shl->cur_wd)));
	if (shl->prompt != NULL)
		ft_free_safe((void **)(&(shl->prompt)));
	rl_clear_history();
	if (shl->stdio[0] != STDIN_FILENO && shl->stdio[0] != -1)
	{
		if (close(shl->stdio[0]) < 0)
			exit_early(NULL, NULL, ERRMSG_CLOSE);
	}
	if (shl->stdio[1] != STDOUT_FILENO && shl->stdio[1] != -1)
	{
		if (close(shl->stdio[1]) < 0)
			exit_early(NULL, NULL, ERRMSG_CLOSE);
	}
}
