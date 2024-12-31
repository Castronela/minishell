/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils_2.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dstinghe <dstinghe@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/26 20:35:34 by dstinghe          #+#    #+#             */
/*   Updated: 2024/12/26 20:47:37 by dstinghe         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

bool	is_redir(const char *str, const size_t index);
bool	is_control(const char *str, const size_t index);
bool	is_special_param(const char *str, const size_t index);
size_t	find_longest_match_length(const char *str, const char *pattern[]);

bool	is_redir(const char *str, const size_t index)
{
	return (find_longest_match_length(&str[index],
			(const char *[]){REDIRECTION_OPERATORS, NULL}));
}

bool	is_control(const char *str, const size_t index)
{
	return (find_longest_match_length(&str[index],
			(const char *[]){CONTROL_OPERATORS, NULL}));
}

bool	is_special_param(const char *str, const size_t index)
{
	return (find_longest_match_length(&str[index],
			(const char *[]){SPECIAL_OPERATORS, NULL}));
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
