/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   syntax_validation.c                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dstinghe <dstinghe@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/13 13:39:18 by dstinghe          #+#    #+#             */
/*   Updated: 2025/01/06 19:48:47 by dstinghe         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

bool	is_valid_quotation(t_shell *shell);
bool	is_valid_control_1(t_shell *shell);
bool	is_valid_control_2(t_shell *shell);
bool	is_redir_target_valid(t_shell *shell, const char *redir_target);

/*
Returns true if all open quotes from cmdline are closed
	- on failure prints error message, sets exit code and resets cmd vars
	- on success sets exit code to 0
*/
bool	is_valid_quotation(t_shell *shell)
{
	char open_qt[2];

	open_qt[1] = 0;
	if (shell->open_qt)
	{
		open_qt[0] = shell->open_qt;
		if (cursor_mv_back(STDERR_FILENO))
			exit_early(shell, NULL, ERRMSG_WRITE);
		if (ft_fprintf_str(STDERR_FILENO, (const char *[]){ERSHL,
				SYNTX_UNEXP_EOF_MATCH, open_qt, "'\n", NULL}))
			exit_early(shell, NULL, ERRMSG_WRITE);
		if (ft_fprintf_str(STDERR_FILENO, (const char *[]){ERSHL,
				SYNTX_UNEXP_EOF, "\n", NULL}))
			exit_early(shell, NULL, ERRMSG_WRITE);
		shell->exit_code = ERRCODE_SYNTAX;
		return (false);
	}
	return (true);
}

/*
Returns true if control operator is preceeded by a valid command
	- on failure prints error message and sets exit code
*/
bool	is_valid_control_1(t_shell *shell)
{
	t_cmds	*cmd_node;

	cmd_node = shell->cmds_lst;
	while (cmd_node)
	{
		if (cmd_node->cmd_separator && is_control(cmd_node->cmd_separator, 0))
		{
			if (!cmd_node->args && !cmd_node->file_in && !cmd_node->file_out
				&& !cmd_node->heredocs_lst)
			{
				if (ft_fprintf_str(STDERR_FILENO, (const char *[]){ERSHL,
						SYNTX_UNEXP_TOKEN, cmd_node->cmd_separator, "'\n",
						NULL}))
					exit_early(shell, NULL, ERRMSG_WRITE);
				shell->exit_code = ERRCODE_SYNTAX;
				return (false);
			}
		}
		cmd_node = cmd_node->next;
	}
	return (true);
}

/*
Returns true if control operator is followed by a valid command
	- on failure prints error message and sets exit code
*/
bool	is_valid_control_2(t_shell *shell)
{
	t_cmds	*cmd_node;

	cmd_node = shell->cmds_lst;
	while (cmd_node)
	{
		if (cmd_node->cmd_separator && is_control(cmd_node->cmd_separator, 0))
		{
			if (!cmd_node->next)
			{
				if (cursor_mv_back(STDERR_FILENO))
					exit_early(shell, NULL, ERRMSG_WRITE);
				if (ft_fprintf_str(STDERR_FILENO, (const char *[]){ERSHL,
						SYNTX_UNEXP_EOF, "\n", NULL}))
					exit_early(shell, NULL, ERRMSG_WRITE);
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
bool	is_redir_target_valid(t_shell *shell, const char *redir_target)
{
	if (!redir_target)
	{
		if (ft_fprintf_str(STDERR_FILENO, (const char *[]){ERSHL,
				SYNTX_UNEXP_TOKEN, "newline'\n", NULL}))
			exit_early(shell, NULL, ERRMSG_WRITE);
		shell->exit_code = ERRCODE_SYNTAX;
		return (false);
	}
	else if (is_control(redir_target, 0) || is_redir(redir_target, 0)
		|| is_command_sep(redir_target, 0))
	{
		if (ft_fprintf_str(STDERR_FILENO, (const char *[]){ERSHL,
				SYNTX_UNEXP_TOKEN, redir_target, "'\n", NULL}))
			exit_early(shell, NULL, ERRMSG_WRITE);
		shell->exit_code = ERRCODE_SYNTAX;
		return (false);
	}
	return (true);
}
