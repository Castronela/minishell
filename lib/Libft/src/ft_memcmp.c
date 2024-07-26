/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_memcmp.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dstinghe <dstinghe@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/03/11 10:44:46 by dstinghe          #+#    #+#             */
/*   Updated: 2024/03/16 14:28:14 by dstinghe         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

int	ft_memcmp(const void *s1, const void *s2, size_t n)
{
	const unsigned char	*str1;
	const unsigned char	*str2;

	str1 = (const unsigned char *)s1;
	str2 = (const unsigned char *)s2;
	while (n)
	{
		if (*str1 - *str2 != 0)
			return (*str1 - *str2);
		str1++;
		str2++;
		n--;
	}
	return (0);
}

// #include <stdio.h>
// #include <string.h>
// int main()
// {
//     char *s1 = "ateiou";
//     char *s2 = "aeiou";
//     int n = 10;
//     printf("%d\n", ft_memcmp(s1, s2, n));
//     printf("%d", memcmp(s1, s2, n));
// }