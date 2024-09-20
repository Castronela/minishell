/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strdup.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pamatya <pamatya@student.42heilbronn.de    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/03/15 13:38:19 by pamatya           #+#    #+#             */
/*   Updated: 2024/06/12 23:45:59 by pamatya          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/libft.h"

// static size_t ft_strlen1(const char *str);

char	*ft_strdup(const char *s1)
{
	char	*str;
	char	*ptr;
	char	*p;
	size_t	byts;

	str = (char *)s1;
	byts = ft_strlen(str);
	ptr = (char *)malloc((byts + 1) * sizeof(char));
	if (!ptr)
		return (NULL);
	p = ptr;
	while (byts--)
		*ptr++ = *str++;
	*ptr++ = '\0';
	return (p);
}
