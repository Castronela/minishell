/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parse_cmdline.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dstinghe <dstinghe@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/13 13:38:18 by dstinghe          #+#    #+#             */
/*   Updated: 2024/12/13 16:37:29 by dstinghe         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

# include "minishell.h"

void	parser(t_shell *shell);

static void	init_cmd_lst(t_shell *shell, t_cmds *new_cmdnode, size_t *index_cmd);
static void	init_args(t_shell *shell, t_cmds *new_cmdnode, char *argument, size_t *arg_count);
static void	init_redirs(t_shell *shell, t_cmds *new_cmdnode, char *operator, size_t	*index_cmd);

/*
(Main FN) Initializes command structure:
	- reads and extracts tokens from commandline
	- initializes separately every command structure with command data
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
	}
	if (!is_valid_control(shell) || !is_valid_quotation(shell))
		get_normal_input(shell);
}

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

static void	init_args(t_shell *shell, t_cmds *new_cmdnode, char *argument, size_t *arg_count)
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

static void	init_redirs(t_shell *shell, t_cmds *new_cmdnode, char *operator, size_t	*index_cmd)
{
	char	*redir_target;
	char	**cmdnode_fd;

	if (!ft_strncmp(operator, RD_IN, ft_strlen(RD_IN) + 1))
		cmdnode_fd = &new_cmdnode->file_in;
	else if (!ft_strncmp(operator, RD_OUT, ft_strlen(RD_OUT)))
	{
		cmdnode_fd = &new_cmdnode->file_out;
		if (!ft_strncmp(operator, RD_OUT_A, ft_strlen(RD_OUT_A) + 1))
			new_cmdnode->apend = 1;
	}
	else
		cmdnode_fd = &new_cmdnode->hd_str;
	free(operator);
	redir_target = get_next_token(shell, index_cmd);
	*cmdnode_fd = redir_target;
	if (is_redir_target_valid(redir_target) == false)
		get_normal_input(shell);
}
