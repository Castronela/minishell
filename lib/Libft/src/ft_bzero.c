/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_bzero.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dstinghe <dstinghe@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/03/06 11:55:52 by dstinghe          #+#    #+#             */
/*   Updated: 2024/03/16 14:25:04 by dstinghe         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

void	ft_bzero(void *s, size_t n)
{
	char	*ss;

	ss = s;
	while (n > 0)
	{
		*ss = '\0';
		ss++;
		n--;
	}
}

// #include <stdio.h>
// #include <string.h>
// #include <strings.h>
// int main()
// {
// 	char s[50];
// 	strcpy(s, "This is string.h library function");
// 	int n = 7;
// 	// bzero(s, n);
// 	ft_bzero(s, n);
// 	printf("%s\n", s);
// }