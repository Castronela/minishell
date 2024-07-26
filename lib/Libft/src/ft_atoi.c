/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_atoi.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dstinghe <dstinghe@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/03/11 14:31:32 by dstinghe          #+#    #+#             */
/*   Updated: 2024/03/15 11:03:11 by dstinghe         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

static int	str_to_num(const char *str)
{
	int	digit;
	int	num;

	num = 0;
	while (*str >= 48 && *str <= 57)
	{
		digit = *str - '0';
		num = num * 10 + digit;
		str++;
	}
	return (num);
}

int	ft_atoi(const char *str)
{
	int	sign;

	sign = 1;
	while (*str)
	{
		if ((*str >= 9 && *str <= 13) || (*str == 32))
			str++;
		else if ((*str == '+') || (*str == '-'))
		{
			if (*str == '-')
				sign = -sign;
			str++;
			break ;
		}
		else
			break ;
	}
	return (str_to_num(str) * sign);
}

// #include <stdio.h>
// #include <stdlib.h>

// int	main(void)
// {
// 	const char *str = "\t\n -2147483648-\t56432";

// 	printf("%d\n", atoi(str));
// 	printf("%d\n", ft_atoi(str));
// }