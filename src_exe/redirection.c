/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   redirection.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pamatya <pamatya@student.42heilbronn.de    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/21 16:15:01 by pamatya           #+#    #+#             */
/*   Updated: 2025/01/03 21:04:17 by pamatya          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/minishell.h"

int		open_file_fds(t_cmds *cmd);
int		set_redirections(t_shell *shl, t_cmds *cmd);
int		close_fds(t_cmds *cmd);

// data.pipe_fd[0]	-	read end of the pipe, i.e. to read from the pipe
// data.pipe_fd[1]	-	write end of the pipe, i.e. to write to the pipe

int	open_file_fds(t_cmds *cmd)
{
	if (cmd->file_in != NULL)
	{
		if (cmd->fd_in != 0)
			close(cmd->fd_in);
		if ((cmd->fd_in = open(cmd->file_in, O_RDONLY)) == -1)
			return (-1);
	}
	if (cmd->file_out != NULL)
	{
		if (cmd->fd_out != 1)
			close(cmd->fd_out);
		if (cmd->apend)
			cmd->fd_out = open(cmd->file_out, O_CREAT | O_WRONLY | O_APPEND, 0644);
		else
			cmd->fd_out = open(cmd->file_out, O_CREAT | O_WRONLY | O_TRUNC, 0644);
		if (cmd->fd_out == -1)
		{
			if (cmd->fd_in != 0)
				close(cmd->fd_in);
			return (-1);
		}
	}
	return (0);
}

int	set_redirections(t_shell *shl, t_cmds *cmd)
{
	(void)shl;
	if (cmd->fd_in != STDIN_FILENO)
	{
		if ((dup2(cmd->fd_in, STDIN_FILENO)) == -1)
			return (close(cmd->fd_in), -1);
	}
	if (cmd->fd_out != STDOUT_FILENO)
	{
		if ((dup2(cmd->fd_out, STDOUT_FILENO)) == -1)
			return (close(cmd->fd_out), -1);
	}
	return (0);
}

// int	set_redirections(t_shell *shl, t_cmds *cmd)
// {
// 	if (cmd->fd_in != STDIN_FILENO)
// 	{
// 		if ((dup2(cmd->fd_in, STDIN_FILENO)) == -1)
// 			return (-1);
// 		if (close(cmd->fd_in) < 0)
// 			return (-2);
// 	}
// 	if (cmd->fd_out != STDOUT_FILENO)
// 	{
// 		if ((dup2(cmd->fd_out, STDOUT_FILENO)) == -1)
// 			return (-1);
// 		if (close(cmd->fd_out) < 0)
// 			return (-2);
// 	}
// 	return (0);
// }

int	close_fds(t_cmds *cmd)
{

	if (cmd->fd_in != 0)
	{
		if (close(cmd->fd_in) < 0)
			return (-1);
	}
	if (cmd->fd_out != 1)
	{
		if (close(cmd->fd_out) < 0)
			return (-1);
	}
	return (0);
}
