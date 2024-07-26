/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_memcpy.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dstinghe <dstinghe@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/03/06 12:09:42 by dstinghe          #+#    #+#             */
/*   Updated: 2024/03/16 14:28:57 by dstinghe         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

void	*ft_memcpy(void *dst, const void *src, size_t n)
{
	unsigned char		*c_dst;
	const unsigned char	*c_src;

	c_dst = (unsigned char *)dst;
	c_src = (const unsigned char *)src;
	if (!dst && !src)
		return (NULL);
	while (n > 0)
	{
		*c_dst = *c_src;
		c_dst++;
		c_src++;
		n--;
	}
	return (dst);
}

// #include <stdio.h>
// #include <string.h>
// int main()
// {
// 	char src[20] = "abcdefg";
// 	char dest[50];
// 	printf("%s\n", memcpy(((void *)0), ((void *)0), 6));
// 	strcpy(src, "abcdefg");
// 	printf("%s\n", ft_memcpy(((void *)0), ((void *)0), 6));
// }
