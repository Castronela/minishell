/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_putstr.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pamatya <pamatya@student.42heilbronn.de    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/03/31 16:20:58 by pamatya           #+#    #+#             */
/*   Updated: 2024/07/04 04:00:59 by pamatya          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/ft_printf.h"

int	ft_putchr(int fd, unsigned char c)
{
	int	printed;

	printed = 0;
	printed = write(fd, &c, 1);
	return (printed);
}

int	ft_putstr(int fd, char *str)
{
	int		ec;
	int		printed;

	printed = 0;
	if (!str)
		return (printed = write(fd, "(null)", 6), printed);
	while (*str)
	{
		ec = write(fd, str++, 1);
		if (ec == -1)
			return (-1);
		printed += ec;
	}
	return (printed);
}
