/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strlcat.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pamatya <pamatya@student.42heilbronn.de    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/03/11 17:31:00 by pamatya           #+#    #+#             */
/*   Updated: 2024/06/12 23:46:16 by pamatya          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/libft.h"

size_t	ft_strlcat(char *dest, const char *src, size_t buffer)
{
	char	*d;
	size_t	len;

	len = ft_strlen(dest);
	if (len > buffer)
		return (ft_strlen(src) + buffer);
	d = dest + len;
	buffer -= len;
	return ((len + ft_strlcpy(d, src, buffer)));
}
