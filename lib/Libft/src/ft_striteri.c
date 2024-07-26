/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_striteri.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dstinghe <dstinghe@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/03/13 15:29:19 by dstinghe          #+#    #+#             */
/*   Updated: 2024/03/15 11:02:07 by dstinghe         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

void	ft_striteri(char *s, void (*f)(unsigned int, char *))
{
	int	index;

	index = 0;
	if (!s || !f)
		return ;
	while (s[index])
	{
		f(index, &s[index]);
		index++;
	}
}

// #include <stdio.h>
// void print(unsigned int index, char *c)
// {
// 	printf("%c", c[0]);
// }

// int main()
// {
// 	char *s = "me";
// 	ft_striteri(s, print);
// }