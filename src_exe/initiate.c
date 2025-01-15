/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   initiate.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pamatya <pamatya@student.42heilbronn.de    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/17 21:46:09 by pamatya           #+#    #+#             */
/*   Updated: 2025/01/16 18:50:44 by pamatya          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/minishell.h"

void 	init_shell(t_shell *shl, int ac, char **av, char **envp);
void	start_shell(t_shell *shl);
void	clearout(t_shell *shl);

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
		set_signal(shl, 1);
		// shl->cmdline = ft_strdup("ls | grep s | grep src");
		shl->cmdline = readline(shl->prompt);
		if (!shl->cmdline)		// Is this required??
			break ;
		if (parser(shl))
		{
			add_history(shl->cmdline);
			reset_cmd_vars(shl, 1);
			continue ;
		}
		set_signal(shl, 2);
		add_history(shl->cmdline);
        test_print_cmdlst(shl, 30);
		index_cmds(shl);
		// get_binaries(shl);
		mini_execute(shl);
		// test_by_print(shl);
		// test_std_fds(shl);
		// test_printf_fds();
		reset_cmd_vars(shl, 1);
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
