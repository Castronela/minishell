/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils_2.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: david <david@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/26 20:35:34 by dstinghe          #+#    #+#             */
/*   Updated: 2025/01/28 23:35:07 by david            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

bool	is_redir(const char *str, const size_t index);
bool	is_control(const char *str, const size_t index);
bool	is_special_param(const char *str, const size_t index);
size_t	find_longest_match_length(const char *str, const char *pattern[]);
void	map_args(t_shell *shell, t_cmds *cmd, void (*func)(t_shell *, char **));

bool	is_redir(const char *str, const size_t index)
{
	return (find_longest_match_length(&str[index], (const char *[]){RD_IN,
			RD_OUT, RD_OUT_A, RD_HD, NULL}));
}

bool	is_control(const char *str, const size_t index)
{
	return (find_longest_match_length(&str[index], (const char *[]){CT_PIPE,
			CT_AND, NULL}));
}

bool	is_special_param(const char *str, const size_t index)
{
	return (find_longest_match_length(&str[index], (const char *[]){DOLLAR,
			QUESTION_MARK, POUND, NULL}));
}

/*
Returns length of longest 'pattern' that matches 'str'
	(w/o null terminator) or 0,	if no matches found
*/
size_t	find_longest_match_length(const char *str, const char *pattern[])
{
	size_t	index;
	size_t	pattern_len;
	size_t	highest_len;

	highest_len = 0;
	index = -1;
	while (pattern[++index])
	{
		pattern_len = ft_strlen(pattern[index]);
		if (!ft_strncmp(str, pattern[index], pattern_len))
		{
			if (pattern_len > highest_len)
				highest_len = pattern_len;
		}
	}
	return (highest_len);
}

void	map_args(t_shell *shell, t_cmds *cmd, void (*func)(t_shell *, char **))
{
	size_t	index;

	index = -1;
	while (cmd->args && cmd->args[++index])
		func(shell, &(cmd->args[index]));
}
