/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils_1.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dstinghe <dstinghe@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/28 20:46:15 by castronela        #+#    #+#             */
/*   Updated: 2025/01/06 18:31:47 by dstinghe         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

bool	is_quote(const char c);
void	skip_whitespaces(const char *str, size_t *index);
int		append_to_str(char **str, char *append, int append_len);
int		ft_fprintf_str(const int fd, const char *str[]);
int		cursor_mv_back(const int fd);

bool	is_quote(const char c)
{
	return (c == SQ || c == DQ);
}

void	skip_whitespaces(const char *str, size_t *index)
{
	while (str[*index] && str[*index] == SPACE)
		(*index)++;
}

/*
Function to append a string to another;
Set 'append_len' to -1 to append whole string 'append',
otherwise will append 'append_len' number of chars;
Return codes:
0 - normal exit
1 - memory allocation failed
*/
int	append_to_str(char **str, char *append, int append_len)
{
	int	total_len;

	if (!append)
		return (0);
	if (append_len < 0)
		append_len = ft_strlen2(append);
	total_len = ft_strlen2(*str) + append_len;
	*str = ft_recalloc(*str, total_len + 1, 0);
	if (!(*str))
		return (1);
	ft_strlcat(*str, append, total_len + 1);
	return (0);
}

/*
Return codes:
0 - exit success
1 - write function failed
*/
int	ft_fprintf_str(const int fd, const char *str[])
{
	size_t	index;

	index = 0;
	while (str[index])
	{
		if (ft_putstr_fd(str[index], fd) < 0)
			return (1);
		index++;
	}
	return (0);
}

int	cursor_mv_back(const int fd)
{
	if (write(fd, MV_CURSOR_BACK_PREV_LINE, 4) < 0 || write(fd, MV_CURSOR_RIGHT,
			5) < 0)
		return (1);
	return (0);
}
