/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   tokenizer.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dstinghe <dstinghe@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/28 20:46:06 by castronela        #+#    #+#             */
/*   Updated: 2025/01/04 17:33:26 by dstinghe         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int				get_next_token(t_shell *shell, size_t *index_cmd, char **token);

static int		get_argument_end_index(t_shell *shell, size_t *index_cmd);
static size_t	operator_length_at_index(const char *str, const size_t index);

/*
Returns allocated string of next token starting from index 'i_cmdline'
!Note: cmdline != NULL
return:
0 - continue normally
1 - free all cmd vars and restart main prompt
*/
int	get_next_token(t_shell *shell, size_t *index_cmd, char **token)
{
	size_t	operator_len;
	size_t	i_start;

	skip_whitespaces(shell->cmdline, index_cmd);
	i_start = (*index_cmd);
	operator_len = operator_length_at_index(shell->cmdline, *index_cmd);
	if (operator_len > 0)
		(*index_cmd) += operator_len;
	else if (get_argument_end_index(shell, index_cmd))
	{
		if (write(STDOUT_FILENO, "\n", 1) < 0)
			exit_early(shell, NULL, ERRMSG_WRITE);
		return (1);
	}
	*token = NULL;
	if (*index_cmd != i_start)
	{
		*token = ft_substr(shell->cmdline, i_start, ((*index_cmd) - i_start));
		if (!(*token))
			exit_early(shell, NULL, ERRMSG_MALLOC);
	}
	skip_whitespaces(shell->cmdline, index_cmd);
	return (0);
}

/*
0 - continue normally
1 - free all cmd vars and restart main prompt
*/
static int	get_argument_end_index(t_shell *shell, size_t *index_cmd)
{
	int	ret_value;

	while (shell->cmdline[*index_cmd])
	{
		skip_quoted_str(shell, shell->cmdline, index_cmd);
		if (!shell->cmdline[*index_cmd] && shell->open_qt)
		{
			ret_value = secondary_prompt(shell, 1);
			if (ret_value == 1)
			{
				printf("%s `%c'\n", ERRMSG_UNEXP_EOF_MATCH, shell->open_qt);
				return (0);
			}
			else if (ret_value == 2)
				return (1);
		}
		else if (shell->cmdline[*index_cmd] == SPACE)
			break ;
		else if (operator_length_at_index(shell->cmdline, *index_cmd) > 0)
			break ;
		(*index_cmd)++;
	}
	return (0);
}

/*
Returns length of longest operator that matches 'str' from 'index' or 0,
	if no matches found
*/
static size_t	operator_length_at_index(const char *str, const size_t index)
{
	return (find_longest_match_length(&str[index],
			(const char *[]){REDIRECTION_OPERATORS, CONTROL_OPERATORS,
			COMMAND_SEPARATORS, NULL}));
}
