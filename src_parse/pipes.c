/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipes.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dstinghe <dstinghe@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/19 15:02:46 by dstinghe          #+#    #+#             */
/*   Updated: 2025/01/06 15:17:57 by dstinghe         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	init_pipes(t_shell *shell);

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
			cmd_node->next->fd_in = pipe_fds[0];
		}
		cmd_node = cmd_node->next;
	}
}

// /*
// Initializes file desccriptors for piped commands
//     - initializes fd_out of left command with the writing pipe
//     - initializes fd_in of right command with the reading pipe

// !!! Need to check it the first if statement here would still hold for double
// 	pipe inputs like "||"
// */
// void	init_pipes(t_shell *shell)
// {
// 	t_cmds	*cmd_node;
// 	int		total_pipes;
// 	int		i;

// 	cmd_node = shell->cmds_lst;
// 	total_pipes = get_total_cmds(shell, 0);
// 	shell->pipe_fds = malloc(total_pipes * sizeof(int *));
// 	if (!shell->pipe_fds)
// 		exit_early(shell, NULL, ERRMSG_MALLOC);
// 	printf("total pipes:	%d\n", total_pipes);
// 	shell->pipe_fds[total_pipes - 1] = NULL;
// 	i = 0;
// 	while (cmd_node)
// 	{
// 		if (cmd_node->cmd_separator && !ft_strncmp(cmd_node->cmd_separator,
// 				CT_PIPE, ft_strlen(CT_PIPE) + 1))
// 		{
// 			shell->pipe_fds[i] = malloc(2 * sizeof(int));
// 			if (!shell->pipe_fds[i])
// 				exit_early(shell, NULL, ERRMSG_MALLOC);
// 			cmd_node->fd_out = shell->pipe_fds[i][1];
// 			cmd_node->next->fd_in = shell->pipe_fds[i][0];
// 			i++;
// 		}
// 		cmd_node = cmd_node->next;
// 	}
// }
