/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   start_shell.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pamatya <pamatya@student.42heilbronn.de    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/17 21:46:09 by pamatya           #+#    #+#             */
/*   Updated: 2024/12/20 13:43:35 by pamatya          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/minishell.h"

void		start_shell(t_shell *shl);
// static void	print_env(t_shell *shl);
// static void	print_shlvl(t_shell *shl);
// static void	print_cwd(t_shell *shl);
// static void	print_env_paths(t_shell *shl);

void	start_shell(t_shell *shl)
{
	while (1)
	{
		shl->cmdline = readline(shl->prompt);
		if (!shl->cmdline)
			break ;
		if (!(ft_strncmp(shl->cmdline, "exit", 4)))
		{
			free(shl->cmdline);
			break ;
		}
		add_history(shl->cmdline);
		parser(shl);
        heredoc(shl);
		init_pipes(shl);
        test_print_cmdlst(shl, 30);
		reset_cmd_vars(shl, 1);
	}

	// print_env(shl);
	// print_env_paths(shl);
	// print_cwd(shl);
	// print_shlvl(shl);
	
}

void	mini_execute(t_shell *shl)
{
	int		extern_cmds;
	int		cmds_index;

	cmds_index = 0;
	extern_cmds = count_extern_cmds(shl->cmds_lst);
	shl->pid = malloc(extern_cmds * sizeof(pid_t));
	if (!shl->pid)
		exit_early(shl, NULL, "PID malloc failed");
	while (shl->cmds_lst)
	{
		if (is_built_in(*(shl->cmds_lst->args)))
			exec_built_in(shl, shl->cmds_lst);
		else
		{
			if (shl->pid[cmds_index] = fork() < 0)
				exit_early(shl, NULL, "Forking failed");
			if (shl->pid[cmds_index] == 0)
			{
				
			}
			
			cmds_index++;
			
		}
		shl->cmds_lst = shl->cmds_lst->next;
	}
}

int	count_extern_cmds(t_cmds *cmds)
{
	int	count;

	count = 0;
	while (cmds)
	{
		if (!is_built_in(*(cmds->args)))
			count++;
		cmds = cmds->next;
	}
	return (count);
}
