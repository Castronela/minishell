/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_memchr.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dstinghe <dstinghe@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/03/11 10:48:09 by dstinghe          #+#    #+#             */
/*   Updated: 2024/03/15 14:42:43 by dstinghe         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

void	*ft_memchr(const void *s, int c, size_t n)
{
	const unsigned char	*str;

	str = (const unsigned char *)s;
	while (n)
	{
		if (*str == (const unsigned char)c)
			return ((void *)str);
		n--;
		str++;
	}
	return (NULL);
}

// #include <stdio.h>
// #include <string.h>
// int main()
// {
//     char *s = "\200";
//     // int c = ;
//     int n = 10;
//     printf("%s, %p\n", ft_memchr(s, '\200', n), ft_memchr(s, '\200', n));
//     printf("%s, %p", memchr(s, '\200', n), memchr(s, '\200', n));
// }