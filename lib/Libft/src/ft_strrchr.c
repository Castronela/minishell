/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strrchr.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dstinghe <dstinghe@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/03/11 10:31:23 by dstinghe          #+#    #+#             */
/*   Updated: 2024/03/15 11:01:18 by dstinghe         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

char	*ft_strrchr(const char *s, int c)
{
	int	index;

	index = 0;
	while (*s)
	{
		index++;
		s++;
	}
	while (index >= 0)
	{
		if (*s == (char)c)
			return ((char *)s);
		index--;
		s--;
	}
	return (0);
}

// #include <stdio.h>
// #include <string.h>
// int main()
// {
//     char *s = "iello- there";
//     char c = 'i';
//     printf("%s, %p\n", ft_strrchr(s, c), ft_strrchr(s, c));
//     printf("%s, %p", strrchr(s, c), strrchr(s, c));
// }