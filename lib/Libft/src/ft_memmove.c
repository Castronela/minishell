/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_memmove.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dstinghe <dstinghe@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/03/06 12:58:42 by dstinghe          #+#    #+#             */
/*   Updated: 2024/03/15 15:35:43 by dstinghe         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

void	*ft_memmove(void *dst, const void *src, size_t n)
{
	char		*c_dst;
	const char	*c_src;
	size_t		i;

	c_dst = (char *)dst;
	c_src = (const char *)src;
	if (!dst && !src)
		return (NULL);
	if (c_dst > c_src)
	{
		while (n--)
			c_dst[n] = c_src[n];
	}
	else
	{
		i = 0;
		while (i < n)
		{
			c_dst[i] = c_src[i];
			i++;
		}
	}
	return (dst);
}

// #include <stdio.h>
// #include <string.h>

// int	main(void)
// {
// 	char	dest1[50] = "hi there";
// 	char	dest2[50] = "hi there";
// 	char	src[20] = "Hello, World!";

// 	printf("%s\n", memmove(src + 2, src, 5));
// 	strcpy(src, "Hello, World!");
// 	printf("%s\n", ft_memmove(src + 2, src, 5));
// }