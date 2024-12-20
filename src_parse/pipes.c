/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipes.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dstinghe <dstinghe@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/19 15:02:46 by dstinghe          #+#    #+#             */
/*   Updated: 2024/12/19 16:29:48 by dstinghe         ###   ########.fr       */
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
		if (cmd_node->ctl_operator && !ft_strncmp(cmd_node->ctl_operator,
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
