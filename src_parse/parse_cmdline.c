/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parse_cmdline.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dstinghe <dstinghe@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/13 13:38:18 by dstinghe          #+#    #+#             */
/*   Updated: 2024/12/18 16:25:16 by dstinghe         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void		parser(t_shell *shell);

static void	init_cmd_lst(t_shell *shell, t_cmds *new_cmdnode,
				size_t *index_cmd);
static void	init_args(t_shell *shell, t_cmds *new_cmdnode, char *argument,
				size_t *arg_count);
static void	init_redirs(t_shell *shell, t_cmds *new_cmdnode, char *operator,
				size_t * index_cmd);
static char	**get_heredoc_del_pt(t_shell *shell, t_cmds *new_cmdnode,
				char *operator);

/*
(Main FN) Initializes 'shell->cmds_lst':
	- creates new node for linked list 'shell->cmds_lst'
	- attaches new node to end of linked list
	- initializes new node
	- does syntax check for control operators and quoted strings
*/
void	parser(t_shell *shell)
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
		init_cmd_lst(shell, new_cmdnode, &index_cmd);
		var_expand_args(shell, new_cmdnode);
	}
	if (!is_valid_control(shell) || !is_valid_quotation(shell))
	{
		reset_cmd_vars(shell, 1);
		start_shell(shell);
	}
}

/*
Initializes 'new_cmdnode' with tokens from a single command:
	- reads and extracts tokens from commandline
	- sorts tokens into 'new_cmdnode', according to their type (redir, control,
		arg)
	- upon finding a control operator, stores operator and exits function
*/
static void	init_cmd_lst(t_shell *shell, t_cmds *new_cmdnode, size_t *index_cmd)
{
	char	*token;
	size_t	arg_count;

	arg_count = 0;
	token = get_next_token(shell, index_cmd);
	while (token)
	{
		if (is_redir(token, 0))
			init_redirs(shell, new_cmdnode, token, index_cmd);
		else if (is_control(token, 0))
		{
			new_cmdnode->ctl_operator = token;
			break ;
		}
		else
			init_args(shell, new_cmdnode, token, &arg_count);
		token = get_next_token(shell, index_cmd);
	}
}

/*
Initializes 'new_cmdnode->args' with 'argument':
	- adds 'argument' and a tailing NULL to 2d array
*/
static void	init_args(t_shell *shell, t_cmds *new_cmdnode, char *argument,
		size_t *arg_count)
{
	(*arg_count)++;
	new_cmdnode->args = ft_realloc(new_cmdnode->args, sizeof(*new_cmdnode->args)
			* (*arg_count + 1));
	if (!new_cmdnode->args)
	{
		free(argument);
		exit_early(shell, NULL, ERRMSG_MALLOC);
	}
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
static void	init_redirs(t_shell *shell, t_cmds *new_cmdnode, char *operator,
		size_t * index_cmd)
{
	char	*redir_target;
	char	**cmdnode_filept;

	cmdnode_filept = NULL;
	if (!ft_strncmp(operator, RD_HD, ft_strlen(RD_HD)))
		cmdnode_filept = get_heredoc_del_pt(shell, new_cmdnode, operator);
	else if (!ft_strncmp(operator, RD_OUT, ft_strlen(RD_OUT)))
	{
		cmdnode_filept = &new_cmdnode->file_out;
		new_cmdnode->apend = 0;
		if (!ft_strncmp(operator, RD_OUT_A, ft_strlen(RD_OUT_A)))
			new_cmdnode->apend = 1;
	}
	else
		cmdnode_filept = &new_cmdnode->file_in;
	free(operator);
	redir_target = get_next_token(shell, index_cmd);
	if (cmdnode_filept && *cmdnode_filept)
		free(*cmdnode_filept);
	*cmdnode_filept = redir_target;
	if (is_redir_target_valid(redir_target) == false)
	{
		reset_cmd_vars(shell, 1);
		start_shell(shell);
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
