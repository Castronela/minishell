/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strchr.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pamatya <pamatya@student.42heilbronn.de    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/03/12 14:37:45 by pamatya           #+#    #+#             */
/*   Updated: 2024/06/12 23:45:55 by pamatya          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/libft.h"

char	*ft_strchr(const char *str, int c)
{
	char			*s;
	unsigned char	letter;
	int				len;
	int				counter;

	s = (char *)str;
	letter = (unsigned char)c;
	len = 0;
	while (*s++)
		len++;
	counter = 0;
	s = (char *)str;
	while (counter <= len)
	{
		if (*s == letter)
			return (s);
		s++;
		counter++;
	}
	return (NULL);
}
