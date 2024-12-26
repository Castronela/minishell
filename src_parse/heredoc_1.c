/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   heredoc_1.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dstinghe <dstinghe@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/17 15:23:48 by dstinghe          #+#    #+#             */
/*   Updated: 2024/12/26 20:29:34 by dstinghe         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

/*
Loops through every command node and checks for open heredocs
*/
int	heredoc(t_shell *shell)
{
	t_cmds		*cmd_node;
	t_lst_str	*heredoc_node;
	int			flag_expand_vars;

	cmd_node = shell->cmds_lst;
	while (cmd_node)
	{
		heredoc_node = cmd_node->heredocs_lst;
		while (heredoc_node)
		{
			flag_expand_vars = count_closed_quotes(heredoc_node->key);
			remove_closed_quotes(shell, &heredoc_node->key);
			append_to_str(&heredoc_node->key, "\n", -1);
			if (heredoc_get_body(shell, heredoc_node))
			{
				if (write(STDOUT_FILENO, "\n", 1) < 0)
					exit_early(shell, NULL, ERRMSG_WRITE);
				return (1);
			}
			heredoc_body_var_expand(shell, heredoc_node, flag_expand_vars);
			heredoc_node = heredoc_node->next;
		}
		cmd_node = cmd_node->next;
	}
	return (0);
}
