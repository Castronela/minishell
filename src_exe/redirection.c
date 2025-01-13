/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   redirection.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dstinghe <dstinghe@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/21 16:15:01 by pamatya           #+#    #+#             */
/*   Updated: 2025/01/13 18:02:09 by dstinghe         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/minishell.h"

void		open_file_fds(t_shell *shl, t_cmds *cmd);
int			set_redirections(t_shell *shl, t_cmds *cmd);
void		ft_close_cmd_pipe(t_shell *shl, t_cmds *cmd, int mod);
void		ft_close_stdcpy(t_shell *shl, int mod);

static void	heredoc_redirections(t_shell *shl, t_cmds *cmd);

// data.pipe_fd[0]	-	read end of the pipe, i.e. to read from the pipe
// data.pipe_fd[1]	-	write end of the pipe, i.e. to write to the pipe

void	open_file_fds(t_shell *shl, t_cmds *cmd)
{
	if (cmd->file_in != NULL && !cmd->toggle_heredoc)
	{
		if (cmd->fd_in != 0)
			close(cmd->fd_in);
		if ((cmd->fd_in = open(cmd->file_in, O_RDONLY)) == -1)
			exit_early(shl, NULL, ERRMSG_OPEN);
	}
	else if (cmd->toggle_heredoc)
		heredoc_redirections(shl, cmd);
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
			exit_early(shl, NULL, ERRMSG_OPEN);
		}
	}
}

static void heredoc_redirections(t_shell *shl, t_cmds *cmd)
{
	t_lst_str *node;
	int fds[2];
	
	if (pipe(fds) < 0)
		exit_early(shl, NULL, ERRMSG_PIPE);;
	node = ft_lst_last(cmd->heredocs_lst);
	if (write(fds[1], node->val, ft_strlen2(node->val)) < 0)
		exit_early(shl, NULL, ERRMSG_WRITE);
	if (close(fds[1]) < 0)
		exit_early(shl, NULL, ERRMSG_CLOSE);
	cmd->fd_in = fds[0];
}

/*
Function to dup2 the STDIN_FILENO and STDOUT_FILENO when fd_in and fd_out are
not already 0 and 1
*/
int	set_redirections(t_shell *shl, t_cmds *cmd)
{
	(void)shl;
	if (cmd->fd_in != STDIN_FILENO)
	{
		if ((dup2(cmd->fd_in, STDIN_FILENO)) == -1)
			return (printf("1\n"),close(cmd->fd_in), -1);
	}
	if (cmd->fd_out != STDOUT_FILENO)
	{
		if ((dup2(cmd->fd_out, STDOUT_FILENO)) == -1)
			return (printf("2\n"),close(cmd->fd_out), -1);
	}
	return (0);
}

/*
Function to close pipes associated to a command
  - Closes pipes when they are not 0, 1 or -1
  - Closes only cmd->fd_in when mod = 0
  - Closes only cmd->fd_out when mod = 1
  - Closes only cmd->fd_cls when mod = 2
  - Closes the copy of STDIN and STDOUT
  - Closes all fds: cmd->fd_in, cmd->fd_out & both cmd->fd_cls when mod = 3
  - Sets the closed ends value to -1 after closing the file descriptor
*/
void	ft_close_cmd_pipe(t_shell *shl, t_cmds *cmd, int mod)
{
	if (mod == 0 && cmd->fd_in != 0 && cmd->fd_in != -1)
	{
		if (close(cmd->fd_in) < 0)
			exit_early(shl, NULL, ERRMSG_CLOSE);
		cmd->fd_in = -1;
	}
	if (mod == 1 && cmd->fd_out != 1 && cmd->fd_out != -1)
	{
		if (close(cmd->fd_out) < 0)
			exit_early(shl, NULL, ERRMSG_CLOSE);
		cmd->fd_out = -1;
	}
	if (mod == 2 && cmd->fd_cls > 1)
	{
		if (close(cmd->fd_cls) < 0)
			exit_early(shl, NULL, ERRMSG_CLOSE);
		cmd->fd_cls = -1;
	}
}

/*
Function to close the dup'ed copies of stdio (especially for child processes)
  - Closes stdin copy if mod = 0
  - Closes stdout copy if mod = 1
  - Closes both stdin and stdout copies if mode = 2
  
*/
void	ft_close_stdcpy(t_shell *shl, int mod)
{
	if ((mod == 0 || mod ==2) && shl->stdio[0] > 1 &&
			shl->stdio[0] != STDIN_FILENO)
	{
		if (close(shl->stdio[0]) < 0)
			exit_early(shl, NULL, ERRMSG_CLOSE);
		shl->stdio[0] = -1;
	}
	if ((mod == 1 || mod ==2) && shl->stdio[1] > 1 &&
			shl->stdio[1] != STDOUT_FILENO)
	{
		if (close(shl->stdio[1]) < 0)
			exit_early(shl, NULL, ERRMSG_CLOSE);
		shl->stdio[1] = -1;
	}
}
