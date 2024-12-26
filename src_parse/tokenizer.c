/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   tokenizer.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dstinghe <dstinghe@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/28 20:46:06 by castronela        #+#    #+#             */
/*   Updated: 2024/12/26 20:22:40 by dstinghe         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

char			*get_next_token(t_shell *shell, size_t *index_cmd);

static size_t	get_argument_end_index(t_shell *shell, size_t index_cmd);
static size_t	operator_length_at_index(const char *str, const size_t index);

/*
Returns allocated string of next token starting from index 'i_cmdline'
!Note: cmdline != NULL
*/
char	*get_next_token(t_shell *shell, size_t *index_cmd)
{
	size_t	operator_len;
	size_t	i_start;
	char	*token;

	skip_whitespaces(shell->cmdline, index_cmd);
	i_start = (*index_cmd);
	operator_len = operator_length_at_index(shell->cmdline, *index_cmd);
	if (operator_len > 0)
		(*index_cmd) += operator_len;
	else
		(*index_cmd) = get_argument_end_index(shell, *index_cmd);
	if (*index_cmd == i_start)
		return (NULL);
	token = ft_substr(shell->cmdline, i_start, ((*index_cmd) - i_start));
	if (!token)
		exit_early(shell, NULL, ERRMSG_MALLOC);
	skip_whitespaces(shell->cmdline, index_cmd);
	return (token);
}

static size_t	get_argument_end_index(t_shell *shell, size_t index_cmd)
{
	while (shell->cmdline[index_cmd])
	{
		if (is_quote(shell->cmdline[index_cmd]))
		{
			skip_quoted_str(shell, shell->cmdline, &index_cmd);
			if (!shell->cmdline[index_cmd])
				break ;
		}
		else if (shell->cmdline[index_cmd] == SPACE)
			break ;
		else if (operator_length_at_index(shell->cmdline, index_cmd) > 0)
			break ;
		index_cmd++;
	}
	return (index_cmd);
}

/*
Returns length of longest operator that matches 'str' from 'index' or 0,
	if no matches found
*/
static size_t	operator_length_at_index(const char *str, const size_t index)
{
	return (find_longest_match_length(&str[index],
			(const char *[]){REDIRECTION_OPERATORS, CONTROL_OPERATORS, NULL}));
}
