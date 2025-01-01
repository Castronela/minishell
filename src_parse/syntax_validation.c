/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   syntax_validation.c                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dstinghe <dstinghe@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/13 13:39:18 by dstinghe          #+#    #+#             */
/*   Updated: 2025/01/01 21:30:46 by dstinghe         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

bool	is_valid_quotation(t_shell *shell);
bool	is_valid_control(t_shell *shell);
bool	is_redir_target_valid(t_shell *shell, char *redir_target);

/*
Returns true if all open quotes from cmdline are closed
	- on failure prints error message, sets exit code and resets cmd vars
	- on success sets exit code to 0
*/
bool	is_valid_quotation(t_shell *shell)
{
	if (shell->open_qt)
	{
		printf("%s ( %c )\n", ERRMSG_OPEN_QUOTE, shell->open_qt);
		shell->exit_code = ERRCODE_SYNTAX;
		return (false);
	}
	return (true);
}

/*
Returns true if control operator is followed by a valid command
	- on failure prints error message, sets exit code and resets cmd vars
	- on success sets exit code to 0
*/
bool	is_valid_control(t_shell *shell)
{
	t_cmds	*cmd_node;

	cmd_node = shell->cmds_lst;
	while (cmd_node)
	{
		if (cmd_node->ctl_operator && !ft_strncmp(cmd_node->ctl_operator, CT_PIPE, ft_strlen(CT_PIPE) + 1))
		{	
			if (cmd_node->next == NULL || (!cmd_node->args && !cmd_node->file_in
				&& !cmd_node->file_out && !cmd_node->heredocs_lst))
			{
				printf("%s `%s'\n", ERRMSG_INCOMPLETE_CONTROL_OPERATOR,
					cmd_node->ctl_operator);
				shell->exit_code = ERRCODE_SYNTAX;
				return (false);
			}
		}
		cmd_node = cmd_node->next;
	}
	return (true);
}

/*
Returns true if redirection target exists and is not an operator
	- on failure prints error message, sets exit code and resets cmd vars
	- on success sets exit code to 0
*/
bool	is_redir_target_valid(t_shell *shell, char *redir_target)
{
	if (!redir_target)
	{
		printf("%s `%s'\n", ERRMSG_UNEXP_TOKEN, "newline");
		shell->exit_code = ERRCODE_SYNTAX;
		return (false);
	}
	else if (is_control(redir_target, 0) || is_redir(redir_target, 0))
	{
		printf("%s `%s'\n", ERRMSG_UNEXP_TOKEN, redir_target);
		shell->exit_code = ERRCODE_SYNTAX;
		return (false);
	}
	return (true);
}
