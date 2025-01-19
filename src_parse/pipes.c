/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipes.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pamatya <pamatya@student.42heilbronn.de    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/19 15:02:46 by dstinghe          #+#    #+#             */
/*   Updated: 2025/01/19 19:30:17 by pamatya          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	init_pipes(t_shell *shell);
void	init_cmd_pipe(t_shell *shell, t_cmds *cmd);

/*
Initializes file desccriptors for piped commands
	- initializes fd_out of left command with the writing pipe
	- initializes fd_in of right command with the reading pipe
*/
void	init_pipes(t_shell *shell)
{
	t_cmds	*cmd_node;
	int		pipe_fds[2];

	cmd_node = shell->cmds_lst;
	while (cmd_node)
	{
		if (cmd_node->cmd_separator && !ft_strncmp(cmd_node->cmd_separator,
				CT_PIPE, ft_strlen(CT_PIPE) + 1))
		{
			if (pipe(pipe_fds) < 0)
				exit_early(shell, NULL, ERRMSG_PIPE);
			cmd_node->fd_out = pipe_fds[1];
			cmd_node->fd_cls = pipe_fds[0];
			cmd_node->next->fd_in = pipe_fds[0];
		}
		cmd_node = cmd_node->next;
	}
}

/*
Function to open a pipe and store it in the command struct
*/
void	init_cmd_pipe(t_shell *shl, t_cmds *cmd)
{
	int pipe_fd[2];
	
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
