/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   syntax_validation.c                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dstinghe <dstinghe@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/13 13:39:18 by dstinghe          #+#    #+#             */
/*   Updated: 2024/12/13 13:43:09 by dstinghe         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

# include "minishell.h"

bool	is_valid_quotation(t_shell *shell);
bool	is_valid_control(t_shell *shell);
bool	is_redir_target_valid(char *redir_target);


bool	is_valid_quotation(t_shell *shell)
{
	if (shell->open_qt)
	{
		printf("%s ( %c )\n", ERRMSG_OPEN_QUOTE, shell->open_qt);
		return (false);
	}
	return (true);
}

bool	is_valid_control(t_shell *shell)
{
	t_cmds *cmd_node;

	cmd_node = shell->cmds_lst;
	while (cmd_node)
	{
		if (cmd_node->ctl_operator && cmd_node->next == NULL)
		{
			printf("%s `%s'\n", ERRMSG_INCOMPLETE_CONTROL_OPERATOR, cmd_node->ctl_operator);
			return (false) ;
		}
		cmd_node = cmd_node->next;
	}
	return (true);
}

bool	is_redir_target_valid(char *redir_target)
{
	if (!redir_target)
	{
		printf("%s `%s'\n", ERRMSG_UNEXP_TOKEN, "newline");
		return (false);
	}
	else if (is_control(redir_target, 0) || is_redir(redir_target, 0))
	{
		printf("%s `%s'\n", ERRMSG_UNEXP_TOKEN, redir_target);
		return (false);
	}
	return (true);
}
