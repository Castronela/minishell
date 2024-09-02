/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_printf.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pamatya <pamatya@student.42heilbronn.de    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/03/31 15:36:06 by pamatya           #+#    #+#             */
/*   Updated: 2024/07/04 04:07:12 by pamatya          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/ft_printf.h"

int	ft_printf(const char *str, ...)
{
	va_list	ap;
	int		ec;
	int		printed;

	printed = 0;
	va_start(ap, str);
	while (*str)
	{
		if (*str == '%')
		{
			str++;
			if (*str && *str == '%')
				ec = ft_putchr(1, (unsigned char)(*str));
			else
				ec = ft_form(1, ap, *str);
		}
		else
			ec = ft_putchr(1, (unsigned char)(*str));
		if (ec == -1)
			return (-1);
		else
			printed += ec;
		str++;
	}
	return (va_end(ap), printed);
}

int	ft_fprintf(int fd, const char *str, ...)
{
	va_list	ap;
	int		ec;
	int		printed;

	printed = 0;
	va_start(ap, str);
	while (*str)
	{
		if (*str == '%')
		{
			str++;
			if (*str && *str == '%')
				ec = ft_putchr(fd, (unsigned char)(*str));
			else
				ec = ft_form(fd, ap, *str);
		}
		else
			ec = ft_putchr(fd, (unsigned char)(*str));
		if (ec == -1)
			return (-1);
		else
			printed += ec;
		str++;
	}
	return (va_end(ap), printed);
}
