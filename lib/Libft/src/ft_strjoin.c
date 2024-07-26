/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strjoin.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dstinghe <dstinghe@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/03/11 17:22:02 by dstinghe          #+#    #+#             */
/*   Updated: 2024/03/16 13:19:41 by dstinghe         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

static unsigned int	my_strlen(const char *s)
{
	int	len;

	len = 0;
	while (s[len])
		len++;
	return (len);
}

static void	*my_memcpy(void *dst, const void *src, unsigned int n)
{
	char	*c_dst;
	char	*c_src;

	c_dst = (char *)dst;
	c_src = (char *)src;
	if (n > 0)
	{
		*c_dst = *c_src;
		my_memcpy(c_dst + 1, c_src + 1, n - 1);
	}
	return (dst);
}

char	*ft_strjoin(char const *s1, char const *s2)
{
	char	*joined_str;
	int		s1_len;
	int		s2_len;

	if (!s1 || !s2)
		return (NULL);
	s1_len = my_strlen(s1);
	s2_len = my_strlen(s2);
	joined_str = malloc((s1_len + s2_len + 1) * sizeof(char));
	if (joined_str == NULL)
		return (NULL);
	my_memcpy(joined_str, s1, s1_len);
	my_memcpy(joined_str + s1_len, s2, s2_len);
	joined_str[s1_len + s2_len] = '\0';
	return (joined_str);
}

// #include <stdio.h>
// int main()
// {
// 	char *s1 = "Hello,";
// 	char *s2 = "there!";

// 	printf("%s", ft_strjoin(s1, s2));
// }