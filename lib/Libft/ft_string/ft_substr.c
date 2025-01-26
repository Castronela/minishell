/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_substr.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pamatya <pamatya@student.42heilbronn.de    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/03/15 16:06:04 by pamatya           #+#    #+#             */
/*   Updated: 2025/01/24 00:58:50 by pamatya          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/libft.h"


/*
Function that allocates (with malloc(3)) and returns a substring from the string
’s’. The substring begins at index ’start’ and is of maximum size ’len’.
Parameters:
	s		: The string from which to create the substring.
	start	: The start index of the substring in the string ’s’.
	len		: The maximum length of the substring.
*/
char	*ft_substr(char const *s, unsigned int start, size_t len)
{
	char	*str;
	char	*sub;
	size_t	i;
	size_t	len_s;

	str = (char *)s;
	i = 0;
	len_s = ft_strlen(str);
	if (start > len_s)
		len = 0;
	else if ((len_s - start) < len)
		len = len_s - start;
	sub = (char *)malloc((len + 1) * sizeof(char));
	if (!sub)
		return (NULL);
	while (i < len)
	{
		sub[i] = str[start + i];
		i++;
	}
	sub[i] = '\0';
	return (sub);
}
