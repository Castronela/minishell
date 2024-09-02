/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_itoa.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pamatya <pamatya@student.42heilbronn.de    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/03/17 18:15:17 by pamatya           #+#    #+#             */
/*   Updated: 2024/06/12 23:47:40 by pamatya          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/libft.h"

static int	f_numlen(int n)
{
	int		i;
	long	num;

	num = (long)n;
	i = 0;
	if (num < 0)
		i = 1;
	while (num)
	{
		i++;
		num /= 10;
	}
	return (i);
}

static int	f_power(int base, int power)
{
	int		result;

	result = 1;
	if (!power)
		return (1);
	while (power--)
		result *= base;
	return (result);
}

static void	f_tostr(char *str, long ln, int n_len)
{
	int		div;
	char	c;

	if (ln < 0)
	{
		*str++ = '-';
		ln = -ln;
		n_len--;
	}
	if (n_len > 0)
	{
		div = f_power(10, (n_len - 1));
		c = ln / div + '0';
		*str++ = c;
		f_tostr(str, (ln % div), (n_len - 1));
	}
	if (!n_len)
		*str++ = '\0';
}

char	*ft_itoa(int n)
{
	char	*str;
	int		n_len;
	long	ln;

	if (!n)
		return (ft_strdup("0"));
	n_len = f_numlen(n);
	str = (char *)malloc((n_len + 1) * sizeof(char));
	if (!str)
		return (NULL);
	ln = (long)n;
	f_tostr(str, ln, n_len);
	return (str);
}
