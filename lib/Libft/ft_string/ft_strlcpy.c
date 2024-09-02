/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strlcpy.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pamatya <pamatya@student.42heilbronn.de    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/03/10 19:53:35 by pamatya           #+#    #+#             */
/*   Updated: 2024/06/12 23:46:22 by pamatya          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/libft.h"

size_t	ft_strlcpy(char *dest, const char *src, size_t dsize)
{
	char		*d;
	const char	*s;

	d = dest;
	s = src;
	if (dsize)
	{
		while (dsize > 1 && *s)
		{
			*d++ = *s++;
			dsize--;
		}
		*d = '\0';
	}
	return (ft_strlen(src));
}
