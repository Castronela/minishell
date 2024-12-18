/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils_1.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dstinghe <dstinghe@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/28 20:46:15 by castronela        #+#    #+#             */
/*   Updated: 2024/12/17 16:56:38 by dstinghe         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

bool is_quote(const char c);
void skip_whitespaces(const char *str, size_t *index);
void skip_quoted_str(t_shell *shell, const char *str, size_t *index);
bool is_redir(const char *str, const size_t index);
bool is_control(const char *str, const size_t index);
size_t find_longest_match_length(const char *str, const char *pattern[]);


bool is_quote(const char c)
{
	return (c == SQ || c == DQ);		
}

void skip_whitespaces(const char *str, size_t *index)
{
	while(str[*index] && str[*index] == SPACE)
		(*index)++;
}

/*
Modifies 'index' to point to closing quote or null terminator, if no closing quote found 
    - stores opening quote char in 'shell->open_qt', to be used later for syntax check
Note: call ONLY when 'str' at 'index' is an opening quote char
*/
void skip_quoted_str(t_shell *shell, const char *str, size_t *index)
{
	shell->open_qt = str[(*index)++];
	while (str[*index] && str[*index] != shell->open_qt)
		(*index)++;
	if (str[*index] == shell->open_qt)
		shell->open_qt = 0;
}

bool is_redir(const char *str, const size_t index)
{
	return (find_longest_match_length(&str[index], (const char *[]){REDIRECTION_OPERATORS, NULL}));
}

bool is_control(const char *str, const size_t index)
{
	return (find_longest_match_length(&str[index], (const char *[]){CONTROL_OPERATORS, NULL}));
}

/*
Returns length of longest 'pattern' that matches 'str' (w/o null terminator) or 0, if no matches found
*/
size_t find_longest_match_length(const char *str, const char *pattern[])
{
	size_t index;
	size_t pattern_len;
	size_t highest_len;
	
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
