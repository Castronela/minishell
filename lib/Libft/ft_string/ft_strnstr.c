/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strnstr.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pamatya <pamatya@student.42heilbronn.de    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/03/23 20:07:38 by pamatya           #+#    #+#             */
/*   Updated: 2024/06/12 23:46:44 by pamatya          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/libft.h"

static int	f_cmpndl(const char *start, const char *needle, char *stop)
{
	unsigned char	*hay;
	unsigned char	*ndl;
	unsigned char	*stp;
	size_t			len_ndl;
	size_t			i;

	hay = (unsigned char *)start;
	ndl = (unsigned char *)needle;
	stp = (unsigned char *)stop;
	len_ndl = ft_strlen(needle);
	i = 0;
	while (i < len_ndl && hay != stp && (*hay || *ndl))
	{
		if (*hay != *ndl)
			return (-1);
		i++;
		hay++;
		ndl++;
	}
	if (i == len_ndl)
		return (0);
	return (1);
}

char	*ft_strnstr(const char *haystack, const char *needle, size_t look)
{
	char	*hay;
	char	*stop;

	hay = (char *)haystack;
	if (!*needle)
		return (hay);
	stop = hay + look;
	while (*hay && look--)
	{
		if ((f_cmpndl(hay, needle, stop)) == 0)
			return (hay);
		hay++;
	}
	return (NULL);
}
