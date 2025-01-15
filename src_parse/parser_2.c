/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser_2.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dstinghe <dstinghe@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/26 20:45:23 by dstinghe          #+#    #+#             */
/*   Updated: 2025/01/15 16:04:48 by dstinghe         ###   ########.fr       */
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
		ret_code = get_next_token(shell, index_cmd, &token);
		if (ret_code == 1)
			return (1);
		else if (ret_code == 2)
			exit_early(shell, NULL, ERRMSG_MALLOC);
		if (!token)
			break ;
		if (is_control(token, 0) || is_command_sep(token, 0))
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

	ret_code = get_next_token(shell, index_cmd, &target);
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
	ft_lst_addback(&new_cmdnode->redirs, node);
	if (!ft_strncmp(node->key, RD_HD, ft_strlen(RD_HD) + 1))
	{
		free(node->key);
		node->key = node->val;
		node->val = NULL;
	}
	else
		expand_homedir_special_char(shell, &node->val);
}

/*
Initializes 'new_cmdnode->args' with 'argument':
	- adds 'argument' and a tailing NULL to 2d array
*/
static void	init_args(t_shell *shell, t_cmds *new_cmdnode, char *argument)
{
	new_cmdnode->arg_count++;
	new_cmdnode->args = ft_recalloc(new_cmdnode->args,
			sizeof(*new_cmdnode->args) * (new_cmdnode->arg_count + 1),
			sizeof(*new_cmdnode->args) * (new_cmdnode->arg_count));
	if (!new_cmdnode->args)
	{
		free(argument);
		exit_early(shell, NULL, ERRMSG_MALLOC);
	}
	expand_homedir_special_char(shell, &argument);
	var_expansion(shell, &argument);
	remove_closed_quotes(shell, &argument);
	new_cmdnode->args[new_cmdnode->arg_count - 1] = argument;
	new_cmdnode->args[new_cmdnode->arg_count] = NULL;
}
