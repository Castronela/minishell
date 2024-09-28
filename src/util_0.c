/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   util_0.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: castronela <castronela@student.42.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/28 20:46:15 by castronela        #+#    #+#             */
/*   Updated: 2024/09/28 20:46:16 by castronela       ###   ########.fr       */
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
Returns true if str starts with valid CONTROL operator
and sets size of operator
*/
bool	is_op_ctrl(const char *str, ssize_t *size)
{
	const char	*valid_char[] = {OPERATOR_CONTROL};
	bool		is_op;
	ssize_t		len;
	ssize_t		i;

	i = -1;
	is_op = false;
	while (valid_char[++i])
	{
		len = ft_strlen(valid_char[i]);
		if (!strncmp(valid_char[i], str, len))
		{
			if (size && len > *size)
				*size = len;
			is_op = true;
		}
	}
	return (is_op);
}

/*
Returns true if str starts with valid REDIRECTION operator
and sets size of operator
*/
bool	is_op_redir(const char *str, ssize_t *size)
{
	const char	*valid_char[] = {OPERATOR_REDIRECTION};
	bool		is_op;
	ssize_t		len;
	ssize_t		i;

	i = -1;
	is_op = false;
	while (valid_char[++i])
	{
		len = ft_strlen(valid_char[i]);
		if (!strncmp(valid_char[i], str, len))
		{
			if (size && len > *size)
				*size = len;
			is_op = true;
		}
	}
	return (is_op);
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
