/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parse_cmdline.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dstinghe <dstinghe@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/13 13:38:18 by dstinghe          #+#    #+#             */
/*   Updated: 2024/12/25 19:22:04 by dstinghe         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	parser(t_shell *shell);

static int	init_cmd_lst(t_shell *shell, t_cmds *new_cmdnode, size_t *index_cmd);
static void	init_args(t_shell *shell, t_cmds *new_cmdnode, char *argument, size_t *arg_count);
static int	init_redirs(t_shell *shell, t_cmds *new_cmdnode, char *operator, size_t * index_cmd);
static char	**get_heredoc_del_pt(t_shell *shell, t_cmds *new_cmdnode, char *operator);
static void expand_homedir_special_char(t_shell *shell, char **str);

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
		var_expand_args(shell, new_cmdnode);
		remove_args_closed_quotes(shell, new_cmdnode);
	}
	if (!is_valid_control(shell) || !is_valid_quotation(shell))
		return (1);
	return (0);
}

/*
Initializes 'new_cmdnode' with tokens from a single command:
	- reads and extracts tokens from commandline
	- sorts tokens into 'new_cmdnode', according to their type (redir, control,
		arg)
	- upon finding a control operator, stores operator and exits function
*/
static int	init_cmd_lst(t_shell *shell, t_cmds *new_cmdnode, size_t *index_cmd)
{
	char	*token;
	size_t	arg_count;

	arg_count = 0;
	token = get_next_token(shell, index_cmd);
	while (token)
	{
		if (is_redir(token, 0))
		{
			if (init_redirs(shell, new_cmdnode, token, index_cmd))
				return (1);
		}
		else if (is_control(token, 0))
		{
			new_cmdnode->ctl_operator = token;
			break ;
		}
		else
			init_args(shell, new_cmdnode, token, &arg_count);
		token = get_next_token(shell, index_cmd);
	}
	return (0);
}

/*
Initializes 'new_cmdnode->args' with 'argument':
	- adds 'argument' and a tailing NULL to 2d array
*/
static void	init_args(t_shell *shell, t_cmds *new_cmdnode, char *argument, size_t *arg_count)
{
	(*arg_count)++;
	new_cmdnode->args = ft_recalloc(new_cmdnode->args, sizeof(*new_cmdnode->args)
			* (*arg_count + 1), sizeof(*new_cmdnode->args) * (*arg_count));
	if (!new_cmdnode->args)
	{
		free(argument);
		exit_early(shell, NULL, ERRMSG_MALLOC);
	}
	expand_homedir_special_char(shell, &argument);
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
static int	init_redirs(t_shell *shell, t_cmds *new_cmdnode, char *operator, size_t * index_cmd)
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
	expand_homedir_special_char(shell, &redir_target);
	*cmdnode_filept = redir_target;
	if (is_redir_target_valid(shell, redir_target) == false)
		return (1);
	return (0);
}

/*
Returns pointer to heredoc delimiter pointer:
	- creates new node for linked list 'new_cmdnode->heredocs_lst'
	- if creation fails, frees 'operator' before exiting
	- attaches new node to end of linked list
	- returns reference to 'key' var of new node
*/
static char	**get_heredoc_del_pt(t_shell *shell, t_cmds *new_cmdnode, char *operator)
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

static void expand_homedir_special_char(t_shell *shell, char **str)
{
	t_lst_str *oldpwd_node;
	char *new_str;
	size_t oldpwd_len;
	size_t str_len;
	
	if (!str || !*str || (!compare_strings("~", *str, 1) && !compare_strings("~/", *str, 0)))
		return ;
	oldpwd_node = ft_find_node(shell->variables, "HOME", 0, 1);
	oldpwd_len = ft_strlen2(oldpwd_node->val);
	str_len = ft_strlen2(*str) - 1;
	new_str = ft_calloc(oldpwd_len + str_len + 1, sizeof(*new_str));
	if (!new_str)
		exit_early(shell, NULL, ERRMSG_MALLOC);
	ft_strlcpy2(new_str, oldpwd_node->val, oldpwd_len + 1);
	if (str_len)
	{
		printf("passed\n");
		ft_strlcat(new_str, &((*str)[1]), oldpwd_len + str_len + 1);
	}
	free(*str);
	*str = new_str;
}
