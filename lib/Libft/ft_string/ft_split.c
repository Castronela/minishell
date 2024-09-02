/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_split.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pamatya <pamatya@student.42heilbronn.de    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/03/18 19:56:13 by pamatya           #+#    #+#             */
/*   Updated: 2024/07/04 21:13:34 by pamatya          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/libft.h"

static size_t	f_word_count(char const *str, char c)
{
	size_t	wcount;

	wcount = 0;
	while (*str)
	{
		if (*str != c)
		{
			wcount++;
			while (*str && *str != c)
				str++;
		}
		else
			str++;
	}
	return (wcount);
}

static int	f_alloc_and_put(char **split, char const *s, char c)
{
	size_t	line;
	size_t	word;
	size_t	length;

	line = 0;
	while (*s)
	{
		word = 0;
		if (*s != c)
		{
			length = 0;
			while (*(s + length) && *(s + length) != c)
				length++;
			split[line] = malloc((length + 1) * sizeof(char));
			if (!split[line])
				return (-1);
			while (*s && *s != c)
				split[line][word++] = *s++;
			split[line][word] = '\0';
			line++;
		}
		else
			s++;
	}
	return (0);
}

char	**ft_split(char const *s, char c)
{
	char	**split;
	size_t	wcount;

	wcount = f_word_count(s, c);
	split = (char **)malloc((wcount + 1) * sizeof(char *));
	if (!split)
		return (NULL);
	split[wcount] = NULL;
	if (f_alloc_and_put(split, s, c) == -1)
		return (ft_free2d(split), NULL);
	return (split);
}
