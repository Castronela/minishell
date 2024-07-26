/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strlcat.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dstinghe <dstinghe@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/03/11 11:19:26 by dstinghe          #+#    #+#             */
/*   Updated: 2024/03/16 14:32:23 by dstinghe         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

static size_t	my_strlen(const char *str)
{
	size_t	len;

	len = 0;
	while (str[len])
		len++;
	return (len);
}

size_t	ft_strlcat(char *dst, const char *src, size_t buffer)
{
	size_t	dst_len;
	size_t	src_len;

	dst_len = my_strlen(dst);
	src_len = my_strlen(src);
	while (*dst)
		dst++;
	while ((dst_len + 1) < buffer && *src)
	{
		*dst = *src;
		src++;
		dst++;
		buffer--;
	}
	*dst = '\0';
	if (dst_len > buffer)
		return (src_len + buffer);
	return (dst_len + src_len);
}

// #include <stdio.h>
// #include <string.h>
// int main ()
// {
// 	const char *src = "the!";
// 	char dst1[20] = "Hello,";
// 	char dst2[20] = "Hello,";
// 	int size = 12;

// 	printf("%lu\n%s\n", strlcat(dst1, src, size), dst1);
// 	printf("%d\n%s\n", ft_strlcat(dst2, src, size), dst2);
// }