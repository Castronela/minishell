/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_memmove.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pamatya <pamatya@student.42heilbronn.de    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/03/10 19:19:50 by pamatya           #+#    #+#             */
/*   Updated: 2024/06/12 23:44:26 by pamatya          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/libft.h"

void	*ft_memmove(void *dest, const void *src, size_t n)
{
	char		*d;
	const char	*s;

	d = dest;
	s = src;
	if (!(d || s))
		return (NULL);
	if (d < s)
	{
		while (n--)
			*d++ = *s++;
		return (dest);
	}
	else
	{
		d += n;
		s += n;
		while (n--)
			*(--d) = *(--s);
		return (dest);
	}
}
