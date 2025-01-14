/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser_1.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dstinghe <dstinghe@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/13 13:38:18 by dstinghe          #+#    #+#             */
/*   Updated: 2025/01/14 15:49:29 by dstinghe         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static int	open_ctr_op(t_shell *shell, t_cmds *cmd_node, size_t *index_cmd);

/*
(Main FN) Initializes 'shell->cmds_lst':
	- creates new node for linked list 'shell->cmds_lst'
	- attaches new node to end of linked list
	- initializes new node
	- does syntax check for control operators and quoted strings
*/
int	parser(t_shell *shell)
{
	size_t	index_cmd;
	t_cmds	*new_cmdnode;

	index_cmd = 0;
	while (index_cmd < ft_strlen2(shell->cmdline))
	{
		while (index_cmd < ft_strlen2(shell->cmdline))
		{
			new_cmdnode = lst_cmds_newnode(shell);
			lst_cmds_addback(shell, new_cmdnode);
			if (init_cmd_lst(shell, new_cmdnode, &index_cmd))
				return (1);
			if (!is_valid_control_1(shell))
				return (1);
		}
		// test_print_cmdlst(shell, 30);
		// test_print_1cmd(shell, new_cmdnode, 30);
		if (heredoc(shell))
			return (1);
		if (index_cmd >= ft_strlen2(shell->cmdline) && open_ctr_op(shell,
				new_cmdnode, &index_cmd))
			return (1);
	}
	if (!is_valid_quotation(shell) || !is_valid_control_2(shell))
		return (1);
	return (0);
}

static int	open_ctr_op(t_shell *shell, t_cmds *cmd_node, size_t *index_cmd)
{
	if (cmd_node->cmd_separator && is_control(cmd_node->cmd_separator, 0))
	{
		if (secondary_prompt(shell, 0) == 2)
		{
			if (write(STDOUT_FILENO, "\n", 1) < 0)
				exit_early(shell, NULL, ERRMSG_WRITE);
			return (1);
		}
		skip_whitespaces(shell->cmdline, index_cmd);
	}
	return (0);
}
