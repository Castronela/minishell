/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_itoa.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dstinghe <dstinghe@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/03/13 12:42:22 by dstinghe          #+#    #+#             */
/*   Updated: 2024/03/15 14:12:04 by dstinghe         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

static size_t	my_numlen(long long n)
{
	int	len;

	len = 0;
	if (n < 0)
	{
		n = -n;
		len++;
	}
	if (n == 0)
		return (1);
	while (n > 0)
	{
		n /= 10;
		len++;
	}
	return (len);
}

char	*ft_itoa(int n)
{
	char		*num;
	long long	nb;
	int			index;

	nb = (long long)n;
	index = my_numlen(n);
	num = malloc((index + 1) * sizeof(char));
	if (num == NULL)
		return (NULL);
	if (nb == 0)
		num[0] = '0';
	if (nb < 0)
	{
		nb = -nb;
		num[0] = '-';
	}
	num[index] = '\0';
	while (nb > 0)
	{
		index--;
		num[index] = '0' + (nb % 10);
		nb /= 10;
	}
	return (num);
}

// #include <stdio.h>
// int main()
// {
// 	int num = -2147483648;

// 	printf("%s", ft_itoa(num));
// }