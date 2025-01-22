/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipes.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: david <david@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/19 15:02:46 by dstinghe          #+#    #+#             */
/*   Updated: 2025/01/22 05:09:47 by david            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	init_cmd_pipe(t_shell *shell, t_cmds *cmd);

/*
Function to open a pipe and store it in the command struct
*/
void	init_cmd_pipe(t_shell *shl, t_cmds *cmd)
{
	int	pipe_fd[2];

	if (!cmd->next)
		return ;
	if (pipe(pipe_fd))
		exit_early(shl, NULL, ERRMSG_PIPE);
	if (cmd->fd_out == STDOUT_FILENO)
		cmd->fd_out = pipe_fd[1];
	else if (ft_close(pipe_fd[1]) < 0)
		exit_early(shl, NULL, ERRMSG_CLOSE);
	if (cmd->next->fd_in == STDIN_FILENO)
		cmd->next->fd_in = pipe_fd[0];
	else if (ft_close(pipe_fd[0]) < 0)
		exit_early(shl, NULL, ERRMSG_CLOSE);
}
