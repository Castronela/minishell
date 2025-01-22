/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser_1.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: david <david@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/13 13:38:18 by dstinghe          #+#    #+#             */
/*   Updated: 2025/01/22 04:28:50 by david            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int			parser(t_shell *shell);
void		map_args(t_shell *shell, t_cmds *cmd, void (*func)(t_shell *,
					char **));
void		retokenize_args(t_shell *shell, t_cmds *cmd);

static int	open_ctr_op(t_shell *shell, t_cmds *cmd_node, size_t *index_cmd);
static void	add_arg(t_shell *shell, t_cmds *cmd, char *arg);

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
	t_cmds	*new_cmdnode;

	index_cmd = 0;
	while (index_cmd < ft_strlen2(shell->cmdline))
	{
		while (index_cmd < ft_strlen2(shell->cmdline))
		{
			new_cmdnode = lst_cmds_newnode(shell);
			lst_cmds_addback(shell, new_cmdnode);
			if (init_cmd_lst(shell, new_cmdnode, &index_cmd))
				return (1);
			if (!is_valid_control_1(shell))
				return (1);
		}
		if (heredoc(shell))
			return (1);
		if (index_cmd >= ft_strlen2(shell->cmdline) && open_ctr_op(shell,
				new_cmdnode, &index_cmd))
			return (1);
	}
	if (!is_valid_quotation(shell) || !is_valid_control_2(shell))
		return (1);
	return (0);
}

static int	open_ctr_op(t_shell *shell, t_cmds *cmd_node, size_t *index_cmd)
{
	if (cmd_node->cmd_separator && is_control(cmd_node->cmd_separator, 0))
	{
		if (secondary_prompt(shell, 0) == 2)
		{
			if (write(STDOUT_FILENO, "\n", 1) < 0)
				exit_early(shell, NULL, ERRMSG_WRITE);
			return (1);
		}
		skip_whitespaces(shell->cmdline, index_cmd);
	}
	return (0);
}

void	retokenize_args(t_shell *shell, t_cmds *cmd)
{
	char	*token;
	size_t	index;

	if (cmd && cmd->args)
	{
		map_args(shell, cmd, var_expansion);
		free(shell->cmdline);
		shell->cmdline = ft_strdup(*cmd->args);
		ft_free2d(cmd->args);
		cmd->args = NULL;
		if (!shell->cmdline)
			exit_early(shell, NULL, ERRMSG_MALLOC);
		index = 0;
		while (true)
		{
			if (get_next_token(shell, &index, &token, 0))
				exit_early(shell, NULL, ERRMSG_MALLOC);
			if (!token)
				break ;
			add_arg(shell, cmd, token);
		}
		map_args(shell, cmd, expand_homedir_special_char);
	}
	if (cmd && cmd->next)
		retokenize_args(shell, cmd->next);
}

void	map_args(t_shell *shell, t_cmds *cmd, void (*func)(t_shell *, char **))
{
	size_t	index;

	index = -1;
	while (cmd->args && cmd->args[++index])
		func(shell, &(cmd->args[index]));
}

static void	add_arg(t_shell *shell, t_cmds *cmd, char *arg)
{
	cmd->arg_count++;
	cmd->args = ft_recalloc(cmd->args, sizeof(*cmd->args) * (cmd->arg_count
				+ 1), sizeof(*cmd->args) * (cmd->arg_count));
	if (!cmd->args)
	{
		free(arg);
		exit_early(shell, NULL, ERRMSG_MALLOC);
	}
	cmd->args[cmd->arg_count - 1] = arg;
	cmd->args[cmd->arg_count] = NULL;
}
