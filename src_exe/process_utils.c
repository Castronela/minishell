/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec_utils.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pamatya <pamatya@student.42heilbronn.de    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/16 00:47:48 by pamatya           #+#    #+#             */
/*   Updated: 2025/01/16 16:43:22 by pamatya          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/minishell.h"

void	create_pids(t_shell *shl);
int		get_total_cmds(t_shell *shl, int which);
void	restore_std_fds(t_shell *shl);

/*
Function to malloc a pid_t pointer for the required number of pids
*/
void	create_pids(t_shell *shl)
{
	int	total_ext;

	total_ext = get_total_cmds(shl, 1);
	shl->pid = malloc(total_ext * sizeof(pid_t));
	if (!shl->pid)
		exit_early(shl, NULL, "PID malloc failed");
}

/*
Function to get the total number of commands
  - Does so by fetching the cmd_index or exc_index of the last command in the list
  - Does not recount the indices
  - If 'which' is 0, returns total number of (internal + external) commands
  - If 'which' is 1, return total number of external commands only
  - Returns -1 for any other value of 'which'
*/
int	get_total_cmds(t_shell *shl, int which)
{
	int		total;
	t_cmds	*cmd;

	total = 0;
	cmd = shl->cmds_lst;
	while (cmd)
	{
		if (which == 0)
				total = cmd->cmd_index;
		else if (which == 1)
		{
			if (total < cmd->exc_index)
				total = cmd->exc_index;
		}
		else
			return (-1);
		cmd = cmd->next;
	}
	return (total);
}

/*
Function to restore the STDIN_FILENO and STDOUT_FILENO to point to the terminal
*/
void	restore_std_fds(t_shell *shl)
{
	if ((dup2(shl->stdio[0], STDIN_FILENO)) == -1)
		exit_early(shl, NULL, ERRMSG_DUP2);
	if ((dup2(shl->stdio[1], STDOUT_FILENO)) == -1)
		exit_early(shl, NULL, ERRMSG_DUP2);
}
