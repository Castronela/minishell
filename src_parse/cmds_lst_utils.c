/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cmds_lst_utils.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pamatya <pamatya@student.42heilbronn.de    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/13 13:14:00 by dstinghe          #+#    #+#             */
/*   Updated: 2025/01/04 16:41:09 by pamatya          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

t_cmds	*lst_cmds_newnode(t_shell *shell);
void	lst_cmds_addback(t_shell *shell, t_cmds *new_cmdnode);
void	lst_cmds_freelst(t_shell *shell);
void	reset_cmd_vars(t_shell *shell, int free_before);

t_cmds	*lst_cmds_newnode(t_shell *shell)
{
	t_cmds	*new_cmd;

	new_cmd = malloc(sizeof(t_cmds));
	if (!new_cmd)
		exit_early(shell, NULL, ERRMSG_MALLOC);
	new_cmd->cmd_index = 0;
	new_cmd->exc_index = 0;
	new_cmd->bin_path = NULL;
	new_cmd->args = NULL;
	new_cmd->heredocs_lst = NULL;
	new_cmd->fd_in = STDIN_FILENO;
	new_cmd->fd_out = STDOUT_FILENO;
	new_cmd->apend = 0;
	new_cmd->file_in = NULL;
	new_cmd->toggle_heredoc = 0;
	new_cmd->file_out = NULL;
	new_cmd->ctl_operator = NULL;
	new_cmd->next = NULL;
	new_cmd->fd_cls[0] = -1;
	new_cmd->fd_cls[1] = -1;
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
		if (cmd_node->heredocs_lst)
			ft_lst_free(&cmd_node->heredocs_lst);
		if (cmd_node->file_in)
			free(cmd_node->file_in);
		if (cmd_node->file_out)
			free(cmd_node->file_out);
		if (cmd_node->ctl_operator)
			free(cmd_node->ctl_operator);
		cmd_node_free = cmd_node;
		cmd_node = cmd_node->next;
		free(cmd_node_free);
	}
}

/*
Nullifies all command variables
	- if 'free_before' > 0 then frees command variables
	before nullifying them
*/
void	reset_cmd_vars(t_shell *shell, int free_before)
{
	if (free_before)
	{
		if (shell->pid)
			free(shell->pid);
		if (shell->cmdline)
			free(shell->cmdline);
		if (shell->cmds_lst)
			lst_cmds_freelst(shell);
	}
	shell->pid = NULL;
	shell->cmdline = NULL;
	shell->cmds_lst = NULL;
	shell->open_qt = 0;
}
