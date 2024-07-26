/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strdup.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dstinghe <dstinghe@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/03/11 16:34:53 by dstinghe          #+#    #+#             */
/*   Updated: 2024/03/16 17:12:15 by dstinghe         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

char	*ft_strdup(const char *s1)
{
	char	*copy;
	size_t	str_len;
	int		copy_index;

	str_len = 0;
	while (s1[str_len])
		str_len++;
	if (!s1)
		return (NULL);
	copy = malloc((str_len + 1) * sizeof(char));
	if (copy == NULL)
		return (NULL);
	copy_index = 0;
	while (*s1)
	{
		copy[copy_index] = *s1;
		copy_index++;
		s1++;
	}
	copy[copy_index] = '\0';
	return (copy);
}

// #include <stdio.h>
// #include <string.h>
// int main()
// {
// 	const char *s1 = "hi there";

// 	char *res1 = strdup(s1);
// 	char *res2 = ft_strdup(s1);
// 	printf("%s\n", res1);
// 	printf("%s\n", res2);
// }