/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser_2.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: david <david@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/26 20:45:23 by dstinghe          #+#    #+#             */
/*   Updated: 2025/01/28 23:32:07 by david            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static int	init_token(t_shell *shell, t_cmds *new_cmdnode, char *token,
				size_t *index_cmd);
static int	init_redirs(t_shell *shell, t_cmds *new_cmdnode, char *operator,
				size_t *index_cmd);
static void	add_redir_node(t_shell *shell, t_cmds *new_cmdnode,
				t_lst_str *node);
static void	init_args(t_shell *shell, t_cmds *new_cmdnode, char *argument);

/*
Initializes 'new_cmdnode' with tokens from a single command:
	- reads and extracts tokens from commandline
	- sorts tokens into 'new_cmdnode', according to their type (redir, control,
		arg)
	- upon finding a control operator, stores operator and exits function
*/
int	init_cmd_lst(t_shell *shell, t_cmds *new_cmdnode, size_t *index_cmd)
{
	int		ret_code;
	char	*token;

	while (true)
	{
		ret_code = get_next_token(shell, index_cmd, &token, 1);
		if (ret_code == 1)
			return (1);
		else if (ret_code == 2)
			exit_early(shell, NULL, ERRMSG_MALLOC);
		if (!token)
			break ;
		if (is_control(token, 0))
		{
			new_cmdnode->cmd_separator = token;
			break ;
		}
		else if (init_token(shell, new_cmdnode, token, index_cmd))
			return (1);
	}
	return (0);
}

static int	init_token(t_shell *shell, t_cmds *new_cmdnode, char *token,
		size_t *index_cmd)
{
	if (is_redir(token, 0))
	{
		if (init_redirs(shell, new_cmdnode, token, index_cmd))
			return (1);
	}
	else
		init_args(shell, new_cmdnode, token);
	return (0);
}

static int	init_redirs(t_shell *shell, t_cmds *new_cmdnode, char *operator,
		size_t *index_cmd)
{
	int			ret_code;
	char		*target;
	t_lst_str	*node;

	ret_code = get_next_token(shell, index_cmd, &target, 1);
	if (ret_code == 1)
		return (1);
	else if (ret_code == 2)
	{
		free(operator);
		exit_early(shell, NULL, ERRMSG_MALLOC);
	}
	if (is_redir_target_valid(shell, target) == false)
	{
		free(target);
		free(operator);
		return (1);
	}
	node = ft_lst_new(operator, target);
	free(operator);
	free(target);
	if (!node)
		exit_early(shell, NULL, ERRMSG_MALLOC);
	add_redir_node(shell, new_cmdnode, node);
	return (0);
}

static void	add_redir_node(t_shell *shell, t_cmds *new_cmdnode, t_lst_str *node)
{
	if (compare_strings(node->key, RD_HD, 1))
	{
		free(node->key);
		node->key = node->val;
		node->val = NULL;
	}
	else
	{
		expand_homedir_special_char(shell, &node->val);
		var_expansion(shell, &node->val);
		remove_closed_quotes(shell, &node->val);
	}
	ft_lst_addback(&new_cmdnode->redirs, node);
}

static void	init_args(t_shell *shell, t_cmds *new_cmdnode, char *argument)
{
	if (!new_cmdnode->args)
		new_cmdnode->args = ft_calloc(2, sizeof(*new_cmdnode->args));
	if (!new_cmdnode->args || append_to_str(new_cmdnode->args, argument, -1)
		|| append_to_str(new_cmdnode->args, " ", -1))
	{
		free(argument);
		exit_early(shell, NULL, ERRMSG_MALLOC);
	}
	free(argument);
}
