/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strlcpy.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dstinghe <dstinghe@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/03/10 19:53:35 by pamatya           #+#    #+#             */
/*   Updated: 2024/12/17 19:48:10 by dstinghe         ###   ########.fr       */
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

size_t	ft_strlcpy2(char *dest, const char *src, size_t dsize)
{
	char		*d;
	const char	*s;

	if (!src)
		return (0);
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
