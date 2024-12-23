/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   redirection.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pamatya <pamatya@student.42heilbronn.de    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/21 16:15:01 by pamatya           #+#    #+#             */
/*   Updated: 2024/12/23 21:00:42 by pamatya          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/minishell.h"

int		open_file_fds(t_cmds *cmd);
int		set_redirections(t_cmds *cmd);
void	close_fds(t_cmds *cmd);

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
		cmd->fd_out = open(cmd->file_out, O_CREAT | O_WNLY | O_TRUNC, 0644);
		if (cmd->fd_out == -1)
		{
			if (cmd->fd_in != 0)
				close(cmd->fd_in);
			return (-1);
		}
	}
	return (0);
}

int	set_redirections(t_cmds *cmd)
{
	// printf("I am inside set_redirections\n");
	if (cmd->fd_in != STDIN_FILENO)
	{
		// printf("I am still inside set_redirections\n");
		if ((dup2(cmd->fd_in, STDIN_FILENO)) == -1)
			return (-1);
		if (close(cmd->fd_in) < 0)
			return (-1);
			// printf("I am exiting set_redirections\n");
	}
	if (cmd->fd_out != STDOUT_FILENO)
	{
		// printf("I am still inside set_redirections\n");
		if ((dup2(cmd->fd_out, STDOUT_FILENO)) == -1)
			return (-1);
		if (close(cmd->fd_out) < 0)
			return (-1);
			// printf("I am still exiting set_redirections\n");
	}
	return (0);
}

void	close_fds(t_cmds *cmd)
{
	if (cmd->fd_in != 0)
		close(cmd->fd_in);
	if (cmd->fd_out != 1)
		close(cmd->fd_out);
}
