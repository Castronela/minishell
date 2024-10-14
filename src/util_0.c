/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   util_0.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: david <david@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/28 20:46:15 by castronela        #+#    #+#             */
/*   Updated: 2024/10/14 17:27:59 by david            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

/*
Returns true if c is whitespace
*/
bool	is_ws(const char c)
{
	const char	white_space[] = {WHITE_SPACE};
	ssize_t		i;

	i = -1;
	while (white_space[++i])
	{
		if (c == white_space[i])
			return (true);
	}
	return (false);
}

/*
Compares beginning of 'str' to every element of 'valids',
and returns length of matched element;
On multiple matches, returns length of biggest element matched;
If no matches found, returns 0;
Caution: last element of 'valids' must be NULL;
*/
size_t is_chars(const char *str, const char *valids[])
{
	size_t 	oplen;
	size_t	len;
	size_t	i;

	oplen = 0;
	i = -1;
	while (valids[++i])
	{
		len = ft_strlen(valids[i]);
		if (!strncmp(valids[i], str, len))
		{
			if (len > oplen)
				oplen = len;
		}
	}
	return (oplen);
}

/*
Returns true if c is single or double quote
*/
bool	is_qt(const char c)
{
	const char	quote[] = {QUOTE};
	ssize_t		i;

	i = -1;
	while (quote[++i])
	{
		if (c == quote[i])
			return (true);
	}
	return (false);
}

/*
Removes closed quotes from token
*/
void	rm_qt(char *token)
{
	char	qt_open;
	size_t	i_token;
	size_t	len_token;

	qt_open = 0;
	i_token = 0;
	len_token = ft_strlen(token);
	while (i_token < len_token)
	{
		if (qt_open == 0 && is_qt(token[i_token]))
		{
			qt_open = token[i_token];
			ft_memmove(&token[i_token], &token[i_token + 1], --len_token);
			token[len_token] = 0;
			i_token--;
		}
		else if (qt_open == token[i_token])
		{
			qt_open = 0;
			ft_memmove(&token[i_token], &token[i_token + 1], --len_token);
			token[len_token] = 0;
			i_token--;
		}
		i_token++;
	}
}
