/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strncmp.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pamatya <pamatya@student.42heilbronn.de    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/03/11 10:33:24 by dstinghe          #+#    #+#             */
/*   Updated: 2024/08/31 18:09:38 by pamatya          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

int	ft_strncmp(const char *s1, const char *s2, size_t n)
{
	unsigned char	*str1;
	unsigned char	*str2;

	str1 = (unsigned char *)s1;
	str2 = (unsigned char *)s2;
	while (n && (*str1 || *str2))
	{
		if (*str1 > *str2)
			return (1);
		else if (*str1 < *str2)
			return (-1);
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
//     char *s1 = "Hellare ther";
//     char *s2 = "Helloo there";
//     int n = 4;
//     printf("%d", strncmp("Tripouille", "TripouilleX", 42));
//     printf("\n%d", ft_strncmp("Tripouille", "TripouilleX", 42));
// }