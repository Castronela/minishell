/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strtrim.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dstinghe <dstinghe@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/03/13 11:20:44 by dstinghe          #+#    #+#             */
/*   Updated: 2024/03/16 13:14:05 by dstinghe         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

static int	check(const char *c, const char *set)
{
	while (*set)
	{
		if (*c == *set)
			return (1);
		set++;
	}
	return (0);
}

static int	startindex(char const *s1, char const *set)
{
	int	start_index;
	int	s1_index;
	int	set_len;

	set_len = 0;
	while (set[set_len])
		set_len++;
	s1_index = 0;
	start_index = 0;
	while (s1[s1_index])
	{
		if (check(&s1[start_index], set) == 1)
		{
			start_index++;
			s1_index++;
		}
		else
			break ;
	}
	return (start_index);
}

static int	endindex(char const *s1, char const *set)
{
	int	fin_index;
	int	s1_index;
	int	s1_len;
	int	set_len;

	set_len = 0;
	while (set[set_len])
		set_len++;
	s1_len = 0;
	while (s1[s1_len])
		s1_len++;
	s1_index = s1_len - 1;
	if (s1_index < 0)
		s1_index = 0;
	fin_index = s1_len;
	while (s1[s1_index])
	{
		if (check(&s1[s1_index], set) == 1)
			fin_index--;
		else
			break ;
		s1_index--;
	}
	return (fin_index);
}

char	*ft_strtrim(char const *s1, char const *set)
{
	char	*trim_str;
	int		trim_len;
	int		trim_index;

	if (!s1)
		return (NULL);
	trim_len = endindex(s1, set) - startindex(s1, set);
	if (trim_len < 0)
		trim_len = 0;
	trim_str = malloc((trim_len + 1) * sizeof(char));
	if (!trim_str)
		return (NULL);
	trim_index = 0;
	while (trim_index < trim_len)
	{
		trim_str[trim_index] = s1[trim_index + startindex(s1, set)];
		trim_index++;
	}
	trim_str[trim_index] = '\0';
	return ((char *)trim_str);
}

// #include <stdio.h>

// int	main(void)
// {
// 	char s1[] = "  \t \t \n   \n\n\n\t";
// 	char s2[] = "lorem ipsum dolor sit amet";
// 	const char *set = "te";
// 	char *result = ft_strtrim("", "");
// 	printf("%s", result);
// }