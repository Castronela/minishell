/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   heredoc.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dstinghe <dstinghe@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/17 15:23:48 by dstinghe          #+#    #+#             */
/*   Updated: 2025/01/07 20:22:30 by dstinghe         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int			heredoc(t_shell *shell);

static int	heredoc_loop(t_shell *shell, t_cmds *cmd_node);
static int	heredoc_get_body(t_shell *shell, t_lst_str *heredoc_node);
static void	heredoc_body_var_expand(t_shell *shell, t_lst_str *heredoc_node,
				int flag_expand_vars);

/*
Loops through every command node and checks for open heredocs;
Return codes:
0 - exit normally
1 - received SIGINT while prompting
*/
int	heredoc(t_shell *shell)
{
	t_cmds	*cmd_node;

	cmd_node = shell->cmds_lst;
	while (cmd_node)
	{
		if (heredoc_loop(shell, cmd_node))
		{
			if (write(STDOUT_FILENO, "\n", 1) < 0)
				exit_early(shell, NULL, ERRMSG_WRITE);
			return (1);
		}
		cmd_node = cmd_node->next;
	}
	return (0);
}

static int	heredoc_loop(t_shell *shell, t_cmds *cmd_node)
{
	t_lst_str	*heredoc_node;
	int			flag_expand_vars;

	heredoc_node = cmd_node->heredocs_lst;
	while (heredoc_node)
	{
		if (!heredoc_node->val)
		{
			flag_expand_vars = count_closed_quotes(heredoc_node->key);
			remove_closed_quotes(shell, &heredoc_node->key);
			if (append_to_str(&heredoc_node->key, "\n", -1))
				exit_early(shell, NULL, ERRMSG_MALLOC);
			if (heredoc_get_body(shell, heredoc_node))
				return (1);
			heredoc_body_var_expand(shell, heredoc_node, flag_expand_vars);
		}
		heredoc_node = heredoc_node->next;
	}
	return (0);
}

/*
Retrieves all necessary Heredoc body from user
*/
static int	heredoc_get_body(t_shell *shell, t_lst_str *heredoc_node)
{
	char	*input;
	int		hd_pipe[2];

	while (true)
	{
		if (prep_prompt(shell, &hd_pipe, 1))
			return (1);
		input = prompt_read(shell, hd_pipe[0]);
		if (!input)
			break ;
		else if (!ft_strncmp(input, heredoc_node->key,
				ft_strlen(heredoc_node->key) + 1))
			break ;
		else if (append_to_str(&heredoc_node->val, input, -1))
		{
			free(input);
			close(hd_pipe[0]);
			exit_early(shell, NULL, ERRMSG_MALLOC);
		}
		free(input);
	}
	if (input)
		free(input);
	close(hd_pipe[0]);
	return (0);
}

/*
Expands variables of heredoc body ONLY if 'flag_expand_vars' is 0
	- encloses entire heredoc body in double quotes
	- performs variable expansion
	- removes double quotes added earlier
*/
static void	heredoc_body_var_expand(t_shell *shell, t_lst_str *heredoc_node,
		int flag_expand_vars)
{
	char	*new_hd_body;
	size_t	hd_body_len;

	if (flag_expand_vars)
		return ;
	hd_body_len = ft_strlen2(heredoc_node->val);
	new_hd_body = ft_calloc(hd_body_len + 3, sizeof(*new_hd_body));
	if (!new_hd_body)
		exit_early(shell, NULL, ERRMSG_MALLOC);
	new_hd_body[0] = '\"';
	ft_strlcpy2(&new_hd_body[1], heredoc_node->val, hd_body_len + 1);
	new_hd_body[hd_body_len + 1] = '\"';
	var_expansion(shell, &new_hd_body);
	hd_body_len = ft_strlen2(new_hd_body);
	new_hd_body = ft_memmove(new_hd_body, &new_hd_body[1], hd_body_len - 1);
	new_hd_body = ft_recalloc(new_hd_body, hd_body_len - 1, 0);
	new_hd_body[hd_body_len - 2] = 0;
	if (!new_hd_body)
		exit_early(shell, NULL, ERRMSG_MALLOC);
	free(heredoc_node->val);
	heredoc_node->val = new_hd_body;
}
