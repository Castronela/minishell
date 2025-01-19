/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cmds_lst_utils.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: david <david@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/13 13:14:00 by dstinghe          #+#    #+#             */
/*   Updated: 2025/01/19 03:26:33 by david            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

t_cmds	*lst_cmds_newnode(t_shell *shell);
void	lst_cmds_addback(t_shell *shell, t_cmds *new_cmdnode);
void	lst_cmds_freelst(t_shell *shell);
t_cmds *lst_cmds_find_node(t_cmds *cmds, const pid_t pid, const int exit_order);

t_cmds	*lst_cmds_newnode(t_shell *shell)
{
	t_cmds	*new_cmd;

	new_cmd = malloc(sizeof(t_cmds));
	if (!new_cmd)
		exit_early(shell, NULL, ERRMSG_MALLOC);
	*new_cmd = (t_cmds){.fd_in = STDIN_FILENO, .fd_out = STDOUT_FILENO,
		.fd_cls = -1, .pid = -1};
	return (new_cmd);
}

void	lst_cmds_addback(t_shell *shell, t_cmds *new_cmdnode)
{
	t_cmds	*current_cmd;

	if (!shell->cmds_lst)
		shell->cmds_lst = new_cmdnode;
	else
	{
		current_cmd = shell->cmds_lst;
		while (current_cmd->next)
			current_cmd = current_cmd->next;
		current_cmd->next = new_cmdnode;
		new_cmdnode->prev = current_cmd;
	}
}

void	lst_cmds_freelst(t_shell *shell)
{
	t_cmds	*cmd_node;
	t_cmds	*cmd_node_free;

	cmd_node = shell->cmds_lst;
	while (cmd_node)
	{
		if (cmd_node->bin_path)
			free(cmd_node->bin_path);
		if (cmd_node->args)
			ft_free2d(cmd_node->args);
		if (cmd_node->redirs)
			ft_lst_free(&cmd_node->redirs);
		if (cmd_node->cmd_separator)
			free(cmd_node->cmd_separator);
		cmd_node_free = cmd_node;
		cmd_node = cmd_node->next;
		free(cmd_node_free);
	}
}

t_cmds *lst_cmds_find_node(t_cmds *cmds, const pid_t pid, const int exit_order)
{
	while (cmds)
	{
		if (pid && cmds->pid == pid)
			break ;
		else if (exit_order && cmds->exit_order == exit_order)
			break ;
		cmds = cmds->next;
	}
	return (cmds);
}
