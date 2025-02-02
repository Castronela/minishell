/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   redirections.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pamatya <pamatya@student.42heilbronn.de    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/21 16:15:01 by pamatya           #+#    #+#             */
/*   Updated: 2025/02/02 16:04:09 by pamatya          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/minishell.h"

int			set_redirs(t_shell *shl, t_cmds *cmd);
int			dup_std_fds(t_shell *shl, t_cmds *cmd);
void		ft_close_cmd_pipe(t_shell *shl, t_cmds *cmd, int mod);
int			ft_close(int fd);

static void	set_fd_pointer_and_flag(t_cmds *cmd, t_lst_str *node, int **pt_fd,
				int *flag);

int	set_redirs(t_shell *shl, t_cmds *cmd)
{
	int			flag;
	int			*pt_fd;
	t_lst_str	*node;

	node = cmd->redirs;
	while (node)
	{
		set_fd_pointer_and_flag(cmd, node, &pt_fd, &flag);
		if (ft_close(*pt_fd) < 0)
			exit_early(shl, NULL, NULL);
		*pt_fd = open(node->val, flag, 0644);
		if (*pt_fd < 0)
		{
			ft_fprintf_str(STDERR_FILENO, (const char *[]){ERSHL, node->val,
				": ", strerror(errno), "\n", NULL});
			cmd->exit_code = ERRCODE_GENERAL;
			return (1);
		}
		node = node->next;
	}
	return (0);
}

static void	set_fd_pointer_and_flag(t_cmds *cmd, t_lst_str *node, int **pt_fd,
	int *flag)
{
	if (!node->key || compare_strings(node->key, RD_IN, 1))
	{
		*pt_fd = &cmd->fd_in;
		*flag = O_RDONLY;
	}
	else
	{
		*pt_fd = &cmd->fd_out;
		if (compare_strings(node->key, RD_OUT_A, 1))
			*flag = O_CREAT | O_WRONLY | O_APPEND;
		else
			*flag = O_CREAT | O_WRONLY | O_TRUNC;
	}
}

/*
Function to dup2 the STDIN_FILENO and STDOUT_FILENO when fd_in and fd_out are
not already 0 and 1
*/
int	dup_std_fds(t_shell *shl, t_cmds *cmd)
{
	(void)shl;
	if (cmd->fd_in != STDIN_FILENO)
	{
		if ((dup2(cmd->fd_in, STDIN_FILENO)) < 0)
			return (close(cmd->fd_in), -1);
		if (ft_close(cmd->fd_in) < 0)
			return (-1);
	}
	if (cmd->fd_out != STDOUT_FILENO)
	{
		if ((dup2(cmd->fd_out, STDOUT_FILENO)) < 0)
			return (close(cmd->fd_out), -1);
		if (ft_close(cmd->fd_out) < 0)
			return (-1);
	}
	return (0);
}

/*
Function to close pipes associated to a command
  - Closes pipes when they are not 0, 1 or -1
  - Closes cmd->fd_in when mod has 1st byte on
  - Closes cmd->fd_out when mod has 2nd byte on
  - Closes shl->stdio[0] when mod has 3rd byte on
  - Closes shl->stdio[1] when mod has 4th byte on
  - Sets the closed ends value to -1 after closing the file descriptor
*/
void	ft_close_cmd_pipe(t_shell *shl, t_cmds *cmd, int mod)
{
	if (mod & 1 && cmd->fd_in != STDIN_FILENO && cmd->fd_in > -1)
	{
		if (close(cmd->fd_in) < 0)
			exit_early(shl, NULL, ERRMSG_CLOSE);
		cmd->fd_in = -1;
	}
	if (mod >> 1 & 1 && cmd->fd_out != STDOUT_FILENO && cmd->fd_out > -1)
	{
		if (close(cmd->fd_out) < 0)
			exit_early(shl, NULL, ERRMSG_CLOSE);
		cmd->fd_out = -1;
	}
	if (mod >> 2 & 1 && shl->stdio[0] != STDIN_FILENO && shl->stdio[0] > -1)
	{
		if (close(shl->stdio[0]) < 0)
			exit_early(shl, NULL, ERRMSG_CLOSE);
		shl->stdio[0] = -1;
	}
	if (mod >> 3 & 1 && shl->stdio[1] != STDOUT_FILENO && shl->stdio[1] > -1)
	{
		if (close(shl->stdio[1]) < 0)
			exit_early(shl, NULL, ERRMSG_CLOSE);
		shl->stdio[1] = -1;
	}
}

int	ft_close(int fd)
{
	if (fd != STDIN_FILENO && fd != STDOUT_FILENO && fd != STDERR_FILENO
		&& fd >= 0)
	{
		if (close(fd) < 0)
		{
			ft_fprintf_str(STDERR_FILENO, (const char *[]){ERSHL, ERRMSG_CLOSE,
				NULL});
			return (-1);
		}
	}
	return (0);
}
