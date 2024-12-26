/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser_1.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dstinghe <dstinghe@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/13 13:38:18 by dstinghe          #+#    #+#             */
/*   Updated: 2024/12/26 20:46:25 by dstinghe         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

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
	size_t	cmdline_len;
	t_cmds	*new_cmdnode;

	cmdline_len = ft_strlen(shell->cmdline);
	index_cmd = 0;
	while (index_cmd < cmdline_len)
	{
		new_cmdnode = lst_cmds_newnode(shell);
		lst_cmds_addback(shell, new_cmdnode);
		if (init_cmd_lst(shell, new_cmdnode, &index_cmd))
			return (1);
	}
	if (!is_valid_control(shell) || !is_valid_quotation(shell))
		return (1);
	return (0);
}
