/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils_4.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pamatya <pamatya@student.42heilbronn.de    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/16 00:47:48 by pamatya           #+#    #+#             */
/*   Updated: 2025/01/20 15:53:22 by pamatya          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/minishell.h"

void	restore_std_fds(t_shell *shl);
int		is_bash_reserved(char c);
// void	create_pids(t_shell *shl);
// int	get_total_cmds(t_shell *shl, int which);

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

/*
Function to check whether the char parameter is a bash-reserved character
  - Checks the character c within the array of special characters defined in the
	header file as bash-reserved characters using ft_strchr()
  - Returns 0 if the character is not found within the array
  - Returns 1 if the character is found within the array
*/
int	is_bash_reserved(char c)
{
	if (ft_strchr((const char []){BT, BN, DL, AD, SC, PO, PC, SQ, DQ, BS, PP, 
			'\0'}, (unsigned int)c) == NULL)
		return (0);
	return (1);
}

// /*
// Function to malloc a pid_t pointer for the required number of pids
// */
// void	create_pids(t_shell *shl)
// {
// 	int	total_ext;

// 	total_ext = get_total_cmds(shl, 0);
// 	shl->pid = malloc(total_ext * sizeof(pid_t));
// 	if (!shl->pid)
// 		exit_early(shl, NULL, "PID malloc failed");
// }

/*
Function to get the total number of commands
  - Does so by fetching the cmd_index or exc_index of the last command in the list
  - Does not recount the indices
  - If 'which' is 0, returns total number of (internal + external) commands
  - If 'which' is 1, return total number of external commands only
  - Returns -1 for any other value of 'which'
// */
// int	get_total_cmds(t_shell *shl, int which)
// {
// 	int		total;
// 	t_cmds	*cmd;

// 	total = 0;
// 	cmd = shl->cmds_lst;
// 	while (cmd)
// 	{
// 		if (which == 0)
// 				total = cmd->cmd_index;
// 		else if (which == 1)
// 		{
// 			if (total < cmd->exc_index)
// 				total = cmd->exc_index;
// 		}
// 		else
// 			return (-1);
// 		cmd = cmd->next;
// 	}
// 	return (total);
// }
