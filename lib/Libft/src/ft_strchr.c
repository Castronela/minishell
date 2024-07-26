/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strchr.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dstinghe <dstinghe@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/03/11 10:38:37 by dstinghe          #+#    #+#             */
/*   Updated: 2024/03/15 11:00:44 by dstinghe         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

char	*ft_strchr(const char *s, int c)
{
	while (*s)
	{
		if (*s == (char)c)
			return ((char *)s);
		s++;
	}
	if ((char)c == '\0')
		return ((char *)s);
	return (0);
}

// #include <stdio.h>
// #include <string.h>
// int main()
// {
//     char *s = "hello\tthere";
//     char c = '\n';
//     printf("%s, %p\n", ft_strchr(s, c), ft_strchr(s, c));
//     printf("%s, %p", strchr(s, c), strchr(s, c));
// }