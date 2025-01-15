/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   redirection.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pamatya <pamatya@student.42heilbronn.de    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/21 16:15:01 by pamatya           #+#    #+#             */
/*   Updated: 2025/01/15 16:58:41 by pamatya          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/minishell.h"

int 		open_file_fds(t_shell *shl, t_cmds *cmd, t_lst_str *node);
int			set_redirections(t_shell *shl, t_cmds *cmd);
void		ft_close_cmd_pipe(t_shell *shl, t_cmds *cmd, int mod);
void		ft_close_stdcpy(t_shell *shl, int mod);

static void set_fd_pointer_and_flag(t_cmds *cmd, t_lst_str *node, int **pt_fd,
	int *flag);

// static void	heredoc_redirections(t_shell *shl, t_cmds *cmd);

// data.pipe_fd[0]	-	read end of the pipe, i.e. to read from the pipe
// data.pipe_fd[1]	-	write end of the pipe, i.e. to write to the pipe

int open_file_fds(t_shell *shl, t_cmds *cmd, t_lst_str *node)
{
	int flag;
	int *pt_fd;

	while(node)
	{
		set_fd_pointer_and_flag(cmd, node, &pt_fd, &flag);
		if (*pt_fd > STDERR_FILENO)
			close(*pt_fd);
		*pt_fd = open(node->val, flag, 0644);
		if (*pt_fd < 0)
		{
			ft_fprintf_str(STDERR_FILENO, (const char *[]){ERSHL, node->val, 
				": ", strerror(errno), "\n", NULL});
			ft_close_cmd_pipe(shl, cmd, 0);
			ft_close_cmd_pipe(shl, cmd, 1);
			shl->exit_code = ERRCODE_GENERAL;
			return (1);
		}
		node = node->next;
	}
	return (0);
}

static void set_fd_pointer_and_flag(t_cmds *cmd, t_lst_str *node, int **pt_fd,
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
	if (mod == 2 && cmd->fd_cls > STDERR_FILENO)
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
