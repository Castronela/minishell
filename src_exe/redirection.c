/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   redirection.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pamatya <pamatya@student.42heilbronn.de    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/12 16:47:30 by pamatya           #+#    #+#             */
/*   Updated: 2024/12/20 22:00:14 by pamatya          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "include/minishell.h"

// data.pipe_fd[0]	-	read end of the pipe, i.e. to read from the pipe
// data.pipe_fd[1]	-	write end of the pipe, i.e. to write to the pipe

int	redirect(int fd_old, int fd_new)
{
	if ((dup2(fd_new, fd_old)) == -1)
		return (-1);
	return (0);
}

int	check_and_open_files(t_cmds *cmd)
{
	int	fdi;
	int	fdo;
	
	if (cmd->file_in)
	{
		if (fdi = open(cmd->file_in, O_RDONLY) == -1)
			return (-1);;
		close(cmd->fd_in);
		cmd->fd_in = fdi;
	}
	if (cmd->file_out)
	{
		if (fdo = open(cmd->file_out, O_CREAT | O_WRONLY | O_TRUNC, 0644) == -1)
		{
			if (cmd->file_in)
				close(fdi);
			return (-1);
		}
		close(cmd->fd_out);
		cmd->fd_out = fdo;
	}
	return (0);
}

int	set_redirections(t_cmds *cmd)
{
	if (cmd->fd_in != STDIN_FILENO)
	{
		if ((dup2(cmd->fd_in, STDIN_FILENO)) == -1)
			return (-1);
	}
	if (cmd->fd_out != STDOUT_FILENO)
	{
		if ((dup2(cmd->fd_out, STDOUT_FILENO)) == -1)
			return (-1);
	}
	close(cmd->fd_in);
	close(cmd->fd_out);
	return (0);
}

void	close_fds(t_cmds *cmd)
{
	close(cmd->fd_in);
	close(cmd->fd_out);
}