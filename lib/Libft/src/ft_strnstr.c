/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strnstr.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dstinghe <dstinghe@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/03/11 12:47:54 by dstinghe          #+#    #+#             */
/*   Updated: 2024/03/16 14:36:56 by dstinghe         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

static int	my_strcmp(const char *s1, const char *s2, size_t len)
{
	size_t	i;

	i = 0;
	while (*s2 && (i < len))
	{
		if (s1[i] - *s2 != 0)
			return (s1[i] - *s2);
		i++;
		s2++;
		if (*s2 && i == len)
			return (1);
	}
	return (0);
}

char	*ft_strnstr(const char *haystack, const char *needle, size_t len)
{
	char	*hay;

	hay = (char *)haystack;
	if (!*needle)
		return (hay);
	while (len && *hay)
	{
		if (my_strcmp(hay, needle, len) == 0)
			return (hay);
		hay++;
		len--;
	}
	return (0);
}

// #include <stdio.h>
// #include <string.h>

// int	main(void)
// {
// 	const char *haystack = "lorem ipsum dolor sit amet";
// 	const char *needle = "dolor";
// 	int len = 13;

// 	printf("%s", strnstr(haystack, needle, len));
// 	printf("\n%s", ft_strnstr(haystack, needle, len));
// }