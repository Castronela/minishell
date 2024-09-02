/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_form.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pamatya <pamatya@student.42heilbronn.de    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/04 04:04:25 by pamatya           #+#    #+#             */
/*   Updated: 2024/07/04 21:00:07 by pamatya          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/ft_printf.h"

int	ft_form(int fd, va_list arg, char opt)
{
	int	printed;

	printed = 0;
	if (opt == 'c')
		printed = ft_putchr(fd, (unsigned char)va_arg(arg, int));
	else if (opt == 's')
		printed = ft_putstr(fd, va_arg(arg, char *));
	else if (opt == 'p')
		printed = ft_putptr(fd, va_arg(arg, void *));
	else if (opt == 'd' || opt == 'i')
		printed = ft_putdec(fd, va_arg(arg, int));
	else if (opt == 'u')
		printed = ft_putdec(fd, va_arg(arg, unsigned int));
	else if (opt == 'x' || opt == 'X')
		printed = ft_puthex(fd, va_arg(arg, unsigned int), opt);
	return (printed);
}
