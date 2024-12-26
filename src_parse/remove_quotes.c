/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   remove_quotes.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dstinghe <dstinghe@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/18 14:49:40 by dstinghe          #+#    #+#             */
/*   Updated: 2024/12/26 20:33:44 by dstinghe         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	remove_closed_quotes(t_shell *shell, char **str);
size_t	count_closed_quotes(char *str);

/*
Removes all enclosing quotes from 'str'
	- allocates memory for new string
	- iterates through original string
	- copies characters to new string, but skips opening
	or enclosing quotes
*/
void	remove_closed_quotes(t_shell *shell, char **str)
{
	char	*new_str;
	char	open_quote;
	size_t	newstr_len;
	size_t	index[2];

	newstr_len = ft_strlen2(*str) - count_closed_quotes(*str);
	new_str = ft_calloc(newstr_len + 1, sizeof(*new_str));
	if (!new_str)
		exit_early(shell, NULL, ERRMSG_MALLOC);
	open_quote = 0;
	index[0] = -1;
	index[1] = 0;
	while ((*str)[++index[0]])
	{
		if (open_quote == 0 && is_quote((*str)[index[0]]))
			open_quote = (*str)[index[0]];
		else if (open_quote == (*str)[index[0]])
			open_quote = 0;
		else
			new_str[index[1]++] = (*str)[index[0]];
	}
	new_str[newstr_len] = 0;
	free(*str);
	*str = new_str;
}

/*
Counts all enclosing quotes from 'str'
*/
size_t	count_closed_quotes(char *str)
{
	size_t	open_quotes_count;
	char	open_quote;

	open_quotes_count = 0;
	open_quote = 0;
	while (*str)
	{
		if (open_quote == 0 && is_quote(*str))
		{
			open_quote = *str;
			open_quotes_count++;
		}
		else if (*str == open_quote)
		{
			open_quote = 0;
			open_quotes_count++;
		}
		str++;
	}
	return (open_quotes_count);
}
