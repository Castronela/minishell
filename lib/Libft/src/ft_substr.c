/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_substr.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dstinghe <dstinghe@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/03/11 16:57:58 by dstinghe          #+#    #+#             */
/*   Updated: 2024/03/16 17:15:29 by dstinghe         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

char	*ft_substr(char const *s, unsigned int start, size_t len)
{
	char	*substr;
	int		substr_index;
	size_t	s_len;

	if (!s)
		return (NULL);
	s_len = 0;
	while (s[s_len])
		s_len++;
	if (s_len <= start)
		len = 0;
	else if (s_len - start < len)
		len = (s_len - start);
	substr = malloc((len + 1) * sizeof(char));
	if (!substr || !s)
		return (NULL);
	substr_index = 0;
	while (len)
	{
		substr[substr_index++] = s[start++];
		len--;
	}
	substr[substr_index] = '\0';
	return (substr);
}

// #include <stdio.h>
// int main()
// {
// 	const char *s = "01234";
// 	int start = 10;
// 	int len = 10;

// 	printf("%s", ft_substr(s, start, len));
// }