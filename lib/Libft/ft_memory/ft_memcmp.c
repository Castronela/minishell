/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_memcmp.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pamatya <pamatya@student.42heilbronn.de    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/03/12 18:20:59 by pamatya           #+#    #+#             */
/*   Updated: 2024/06/12 23:44:16 by pamatya          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/libft.h"

int	ft_memcmp(const char *str1, const char *str2, size_t n)
{
	const char		*s1;
	const char		*s2;
	unsigned char	c1;
	unsigned char	c2;

	s1 = str1;
	s2 = str2;
	while (n)
	{
		c1 = (unsigned char)*s1;
		c2 = (unsigned char)*s2;
		if (c1 != c2)
			return (c1 - c2);
		s1++;
		s2++;
		n--;
	}
	return (0);
}
