/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   util_0.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: castronela <castronela@student.42.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/28 20:46:15 by castronela        #+#    #+#             */
/*   Updated: 2024/09/29 13:40:03 by castronela       ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static void	check_op(bool *is_op, const char *str, const char *valid_char[], ssize_t *size);

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
Returns true if str starts with valid operator, depending on
'type' flag set and sets length of operator 'len';
Flags:
	RD - redirection,
	HD - here document,
	CT - control
*/
bool	is_op(const char *str, const int type, ssize_t *oplen)
{
	const char	*valid_rd[] = {OPERATOR_REDIRECTION};
	const char	*valid_hd[] = {OPERATOR_HEREDOC};
	const char	*valid_ct[] = {OPERATOR_CONTROL};
	bool is_op;

	*oplen = 0;
	is_op = false;
	if (type & RD)
			check_op(&is_op, str, valid_rd, oplen);
	if (type & HD)
			check_op(&is_op, str, valid_hd, oplen);
	if (type & CT)
			check_op(&is_op, str, valid_ct, oplen);
	return (is_op);
}

/*
Utility for function 'is_op';
Sets 'is_op' to true if beginning of string matches any string
from the 'valid_char' array;
On match, if original 'size' is smaller than len of matched string,
then 'size' is assigned len of matched string;
*/
static void	check_op(bool *is_op, const char *str, const char *valid_char[], ssize_t *oplen)
{
	ssize_t		len;
	ssize_t		i;

	i = -1;
	while (valid_char[++i])
	{
		len = ft_strlen(valid_char[i]);
		if (!strncmp(valid_char[i], str, len))
		{
			if (oplen && len > *oplen)
				*oplen = len;
			*is_op = true;
		}
	}
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
