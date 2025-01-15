/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   heredoc.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: david <david@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/17 15:23:48 by dstinghe          #+#    #+#             */
/*   Updated: 2025/01/15 01:43:33 by david            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int			heredoc(t_shell *shell);

static int	heredoc_loop(t_shell *shell, t_cmds *cmd_node);
static int	heredoc_get_body(t_shell *shell, t_lst_str *heredoc_node, 
	const int flag_expand_vars);
static void	heredoc_body_var_expand(t_shell *shell, char **str,
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
		shell->heredoc_file_no++;
		cmd_node = cmd_node->next;
	}
	return (0);
}

static int	heredoc_loop(t_shell *shell, t_cmds *cmd_node)
{
	t_lst_str	*node;
	int			flag_expand_vars;

	node = cmd_node->redirs;
	while (node)
	{
		if (!node->val && node->key)
		{
			flag_expand_vars = count_closed_quotes(node->key);
			remove_closed_quotes(shell, &node->key);
			if (open_hd_tmp_file(shell, node))
				return (1);
			if (append_to_str(&node->key, "\n", -1))
				exit_early(shell, NULL, ERRMSG_MALLOC); 
			if (heredoc_get_body(shell, node, flag_expand_vars))
				return (1);
			free(node->key);
			node->key = NULL;
			close(shell->tmp_file_fd);
			shell->tmp_file_fd = -1;
		}
		node = node->next;
	}
	return (0);
}


/*
Retrieves all necessary Heredoc body from user
*/
static int	heredoc_get_body(t_shell *shell, t_lst_str *heredoc_node, 
	const int flag_expand_vars)
{
	char	*input;
	int		hd_pipe[2];

	while (true)
	{
		if (prep_prompt(shell, &hd_pipe, 1))
			return (1);
		input = prompt_read(shell, hd_pipe[0]);
		if (!input || !ft_strncmp(input, heredoc_node->key,
				ft_strlen(heredoc_node->key) + 1))
			break ;
		heredoc_body_var_expand(shell, &input, flag_expand_vars);
		if (write(shell->tmp_file_fd, input, ft_strlen2(input)) < 0)
		{
			free(input);
			exit_early(shell, NULL, ERRMSG_WRITE);
		}
		free(input);
	}
	if (input)
		free(input);
	return (0);
}

/*
Expands variables of heredoc body ONLY if 'flag_expand_vars' is 0
	- encloses entire heredoc body in double quotes
	- performs variable expansion
	- removes double quotes added earlier
*/
static void	heredoc_body_var_expand(t_shell *shell, char **str,
		int flag_expand_vars)
{
	char	*new_hd_body;
	size_t	hd_body_len;

	if (flag_expand_vars)
		return ;
	hd_body_len = ft_strlen2(*str);
	new_hd_body = ft_calloc(hd_body_len + 3, sizeof(*new_hd_body));
	if (!new_hd_body)
	{
		free(*str);
		exit_early(shell, NULL, ERRMSG_MALLOC);
	}
	new_hd_body[0] = '\"';
	ft_strlcpy2(&new_hd_body[1], *str, hd_body_len + 1);
	free(*str);
	new_hd_body[hd_body_len + 1] = '\"';
	var_expansion(shell, &new_hd_body);
	hd_body_len = ft_strlen2(new_hd_body);
	new_hd_body = ft_memmove(new_hd_body, &new_hd_body[1], hd_body_len - 1);
	new_hd_body = ft_recalloc(new_hd_body, hd_body_len - 1, 0);
	if (!new_hd_body)
		exit_early(shell, NULL, ERRMSG_MALLOC);
	new_hd_body[hd_body_len - 2] = 0;
	*str = new_hd_body;
}
