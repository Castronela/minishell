/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_split.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dstinghe <dstinghe@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/03/13 11:23:42 by dstinghe          #+#    #+#             */
/*   Updated: 2024/03/16 17:06:34 by dstinghe         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

static int	char_count(char const *str, char c)
{
	int	len;

	len = 0;
	while (str[len])
	{
		if (str[len] == c)
			break ;
		len++;
	}
	return (len);
}

static int	word_count(char const *str, char c)
{
	int	count;
	int	index;

	index = 0;
	count = 0;
	if (str[0] && str[0] != c)
	{
		count++;
		index++;
	}
	while (str[index])
	{
		if ((str[index] != c) && (str[index - 1] == c))
			count++;
		index++;
	}
	return (count);
}

static char	*my_strcpy(const char *src, size_t dst_size)
{
	size_t	index;
	char	*dst;

	dst = malloc((dst_size + 1) * sizeof(char));
	if (!dst)
		return (NULL);
	index = 0;
	while (index < dst_size)
	{
		dst[index] = src[index];
		index++;
	}
	dst[index] = '\0';
	return (dst);
}

void	free_memory(char **array)
{
	int	i;

	i = 0;
	while (array[i] != NULL)
	{
		free(array[i]);
		i++;
	}
	free(array);
}

char	**ft_split(char const *s, char c)
{
	char	**split;
	int		rows;
	int		word_nr;

	word_nr = word_count(s, c);
	if (!s)
		return (NULL);
	split = malloc((word_nr + 1) * sizeof(char *));
	if (!split)
		return (NULL);
	split[word_nr] = NULL;
	rows = 0;
	while (rows < word_nr)
	{
		if (*s != c)
		{
			split[rows] = my_strcpy(s, char_count(s, c));
			if (split[rows] == NULL)
				return (free_memory(split), NULL);
			s += char_count(s, c);
			rows++;
		}
		s++;
	}
	return (split);
}

// #include <stdio.h>

// int	main(void)
// {
// 	char const *s = "xxxxxxxxhello!";
// 	char c = 'x';
// 	char **result;

// 	result = ft_split(s, c);
// 	printf("%d\n", word_count(s, c));
// 	printf("%d\n", char_count(s, c));
// 	for (int i = 0; i < word_count(s, c); i++)
// 	{
// 		printf("%s\n", result[i]);
// 	}
// }