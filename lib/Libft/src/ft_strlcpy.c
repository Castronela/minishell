/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strlcpy.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dstinghe <dstinghe@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/03/11 10:59:03 by dstinghe          #+#    #+#             */
/*   Updated: 2024/03/16 14:34:08 by dstinghe         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

size_t	ft_strlcpy(char *dst, const char *src, size_t dstsize)
{
	size_t	fin_size;
	size_t	i;

	fin_size = 0;
	while (src[fin_size])
		fin_size++;
	if (dstsize > 0)
	{
		i = 0;
		while ((i + 1 < dstsize) && src[i])
		{
			dst[i] = src[i];
			i++;
		}
		dst[i] = '\0';
	}
	return (fin_size);
}

// #include <stdio.h>
// #include <stdlib.h>
// #include <string.h>
// int main()
// {
// 	const char *src = "hello,th";
// 	char *dst1 = malloc(0 * sizeof(char));
// 	char *dst2 = malloc(0 * sizeof(char));
// 	int size = 5;

// 	printf("%lu\n%s\n", strlcpy(dst1, src, size), dst1);
// 	printf("%d\n%s", ft_strlcpy(dst2, src, size), dst2);
// }