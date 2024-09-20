/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_utils.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pamatya <pamatya@student.42heilbronn.de    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/03/31 20:27:23 by pamatya           #+#    #+#             */
/*   Updated: 2024/07/04 04:01:05 by pamatya          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/ft_printf.h"

void	ft_revstr(char *str)
{
	char	*s;
	char	tmp;
	int		i;
	int		len;	

	i = 0;
	len = ft_strlen(str) - 1;
	s = str;
	while (i < len)
	{
		tmp = s[i];
		s[i] = str[len];
		str[len] = tmp;
		i++;
		len--;
	}
}

void	ft_strtoup(char *str)
{
	unsigned char	letter;
	char			*s;

	s = str;
	while (*s)
	{
		letter = (unsigned char)(*s);
		if (letter >= 'a' && letter <= 'z')
			*s -= 32;
		s++;
	}
}

void	ft_basetostr(long num_arg, char *str, long base)
{
	char			*strptr;
	long			num;

	num = num_arg;
	strptr = str;
	if (num == 0)
		*strptr++ = '0';
	if (num < 0)
	{
		*strptr++ = '-';
		num = -num;
	}
	while (num)
	{
		if ((num % base) < 10)
			*strptr++ = (num % base) + '0';
		else if ((num % base) < 16)
			*strptr++ = (num % base) - 10 + 'a';
		num /= base;
	}
	*strptr = '\0';
	if (num_arg < 0)
		ft_revstr(str + 1);
	else
		ft_revstr(str);
}
