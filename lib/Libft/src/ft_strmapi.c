/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strmapi.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dstinghe <dstinghe@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/03/13 14:37:30 by dstinghe          #+#    #+#             */
/*   Updated: 2024/03/15 11:02:19 by dstinghe         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

char	*ft_strmapi(char const *s, char (*f)(unsigned int, char))
{
	char	*strmapi;
	int		s_len;
	int		index;

	if (!s || !f)
		return (0);
	s_len = 0;
	while (s[s_len])
		s_len++;
	strmapi = malloc((s_len + 1) * sizeof(char));
	if (strmapi == NULL)
		return (NULL);
	index = 0;
	while (s[index])
	{
		strmapi[index] = f(index, s[index]);
		index++;
	}
	strmapi[index] = '\0';
	return (strmapi);
}

// char	ft_toupper(unsigned int index, char c)
// {
// 	if (c >= 97 && c <= 122)
// 		return (c += - 'a' + 'A');
// 	return (c);
// }

// #include <stdio.h>
// int main()
// {
// 	char *s = "hello there";

// 	printf("%s", ft_strmapi(s, ft_toupper));
// }