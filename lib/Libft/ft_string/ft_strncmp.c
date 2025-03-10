/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strncmp.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pamatya <pamatya@student.42heilbronn.de    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/03/12 15:39:09 by pamatya           #+#    #+#             */
/*   Updated: 2025/01/23 14:21:53 by pamatya          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/libft.h"

int	ft_strncmp(const char *str1, const char *str2, size_t n)
{
	unsigned char	*s1;
	unsigned char	*s2;

	s1 = (unsigned char *)str1;
	s2 = (unsigned char *)str2;
	while (n-- && (*s1 || *s2))
	{
		if (*s1 < *s2)
			return (-1);
		else if (*s2 < *s1)
			return (1);
		s1++;
		s2++;
	}
	return (0);
}

int	ft_strcmp(const char *str1, const char *str2)
{
	unsigned char	*s1;
	unsigned char	*s2;

	s1 = (unsigned char *)str1;
	s2 = (unsigned char *)str2;
	while (*s1 & *s2)
		if (*s1 != *s2)
			return (*s1++ - *s2++);
	return (*s1 - *s2);
}
