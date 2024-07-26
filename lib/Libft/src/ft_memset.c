/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_memset.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dstinghe <dstinghe@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/03/06 11:20:20 by dstinghe          #+#    #+#             */
/*   Updated: 2024/03/16 14:29:45 by dstinghe         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

void	*ft_memset(void *b, int c, size_t len)
{
	unsigned char	*bb;

	bb = (unsigned char *)b;
	while (len)
	{
		*bb = (unsigned char)c;
		bb++;
		len--;
	}
	return (b);
}

// #include <stdio.h>
// #include <stdlib.h>
// #include <string.h>
// int main()
// {
// 	char b[50];
// 	char *a = (char*)malloc(sizeof(char) * (0xF0000 + 1));
// 	a = 0;
// 	strcpy(b, "This is string.h library function");
// 	char c = '$';
// 	int len = 7;
// 	printf("%s\n", ft_memset(a, '\5', 0xF0000));
// 	// printf("%s\n", memset(a, '\5', 0xF0000));
// }