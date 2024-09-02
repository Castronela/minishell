/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strrchr.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pamatya <pamatya@student.42heilbronn.de    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/03/12 15:15:49 by pamatya           #+#    #+#             */
/*   Updated: 2024/06/12 23:46:51 by pamatya          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/libft.h"

char	*ft_strrchr(const char *str, int c)
{
	char			*s;
	unsigned char	letter;
	int				len;
	int				counter;

	s = (char *)str;
	letter = (unsigned char)c;
	len = 0;
	while (*s)
	{
		len++;
		s++;
	}
	counter = 0;
	while (counter <= len)
	{
		if (*s == letter)
			return (s);
		s--;
		counter++;
	}
	return (NULL);
}
