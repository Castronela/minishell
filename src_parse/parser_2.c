/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser_2.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pamatya <pamatya@student.42heilbronn.de    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/26 20:45:23 by dstinghe          #+#    #+#             */
/*   Updated: 2025/01/07 18:58:19 by pamatya          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static void	init_args(t_shell *shell, t_cmds *new_cmdnode, char *argument,
				size_t *arg_count);
static int	init_redirs(t_shell *shell, t_cmds *new_cmdnode, char *operator,
				size_t *index_cmd);
static char	**get_redir_pt(t_shell *shell, t_cmds *new_cmdnode, char *operator);
static char	**get_heredoc_del_pt(t_shell *shell, t_cmds *new_cmdnode,
				char *operator);

/*
Initializes 'new_cmdnode' with tokens from a single command:
	- reads and extracts tokens from commandline
	- sorts tokens into 'new_cmdnode', according to their type (redir, control,
		arg)
	- upon finding a control operator, stores operator and exits function
*/
int	init_cmd_lst(t_shell *shell, t_cmds *new_cmdnode, size_t *index_cmd)
{
	char	*token;
	size_t	arg_count;

	arg_count = 0;
	if (get_next_token(shell, index_cmd, &token))
		return (1);
	while (token)
	{
		if (is_redir(token, 0))
		{
			if (init_redirs(shell, new_cmdnode, token, index_cmd))
				return (1);
		}
		else if (is_control(token, 0) || is_command_sep(token, 0))
		{
			new_cmdnode->cmd_separator = token;
			break ;
		}
		else
			init_args(shell, new_cmdnode, token, &arg_count);
		if (get_next_token(shell, index_cmd, &token))
			return (1);
	}
	return (0);
}

/*
Initializes 'new_cmdnode->args' with 'argument':
	- adds 'argument' and a tailing NULL to 2d array
*/
static void	init_args(t_shell *shell, t_cmds *new_cmdnode, char *argument,
		size_t *arg_count)
{
	(*arg_count)++;
	new_cmdnode->args = ft_recalloc(new_cmdnode->args,
			sizeof(*new_cmdnode->args) * (*arg_count + 1),
			sizeof(*new_cmdnode->args) * (*arg_count));
	if (!new_cmdnode->args)
	{
		free(argument);
		exit_early(shell, NULL, ERRMSG_MALLOC);
	}
	expand_homedir_special_char(shell, &argument);
	var_expansion(shell, &argument);
	remove_closed_quotes(shell, &argument);
	new_cmdnode->args[(*arg_count) - 1] = argument;
	new_cmdnode->args[(*arg_count)] = NULL;
}

/*
Initializes 'new_cmdnode' with redirection targets:
	- checks redirection type of 'operator'
	- retrieves next token and stores it
	into 'new_cmdnode' as redirection target
	- does syntax check for redirection operators
*/
static int	init_redirs(t_shell *shell, t_cmds *new_cmdnode, char *operator,
		size_t *index_cmd)
{
	char	*redir_target;
	char	**cmdnode_filept;

	cmdnode_filept = get_redir_pt(shell, new_cmdnode, operator);
	free(operator);
	if (get_next_token(shell, index_cmd, &redir_target))
		return (1);
	if (cmdnode_filept && *cmdnode_filept)
		free(*cmdnode_filept);
	if (redir_target)
		expand_homedir_special_char(shell, &redir_target);
	*cmdnode_filept = redir_target;
	if (is_redir_target_valid(shell, redir_target) == false)
		return (1);
	return (0);
}

static char	**get_redir_pt(t_shell *shell, t_cmds *new_cmdnode, char *operator)
{
	if (!ft_strncmp(operator, RD_HD, ft_strlen(RD_HD) + 1))
	{
		new_cmdnode->toggle_heredoc = 1;
		return (get_heredoc_del_pt(shell, new_cmdnode, operator));
	}
	else if (!ft_strncmp(operator, RD_IN, ft_strlen(RD_IN) + 1))
	{
		new_cmdnode->toggle_heredoc = 0;
		return (&new_cmdnode->file_in);
	}
	else if (!ft_strncmp(operator, RD_OUT, ft_strlen(RD_OUT) + 1))
	{
		new_cmdnode->apend = 0;
		return (&new_cmdnode->file_out);
	}
	else
	{
		new_cmdnode->apend = 1;
		return (&new_cmdnode->file_out);
	}
}

/*
Returns pointer to heredoc delimiter pointer:
	- creates new node for linked list 'new_cmdnode->heredocs_lst'
	- if creation fails, frees 'operator' before exiting
	- attaches new node to end of linked list
	- returns reference to 'key' var of new node
*/
static char	**get_heredoc_del_pt(t_shell *shell, t_cmds *new_cmdnode,
		char *operator)
{
	t_lst_str	*heredoc_node;

	heredoc_node = ft_lst_new(NULL, NULL);
	if (!heredoc_node)
	{
		free(operator);
		exit_early(shell, NULL, ERRMSG_MALLOC);
	}
	ft_lst_addback(&new_cmdnode->heredocs_lst, heredoc_node);
	return (&heredoc_node->key);
}
