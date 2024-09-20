/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_putptr.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pamatya <pamatya@student.42heilbronn.de    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/03/31 16:20:46 by pamatya           #+#    #+#             */
/*   Updated: 2024/07/04 04:00:54 by pamatya          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/ft_printf.h"

static void	ft_toptrlow(unsigned long addr, char *hex)
{
	char			*hexptr;

	hexptr = hex;
	if (addr == 0)
		*hexptr++ = '0';
	while (addr)
	{
		if ((addr % 16) < 10)
			*hexptr++ = (addr % 16) + '0';
		else if ((addr % 16) < 16)
			*hexptr++ = (addr % 16) - 10 + 'a';
		addr /= 16;
	}
	*hexptr = '\0';
	ft_revstr(hex);
}

int	ft_putptr(int fd, void *address)
{
	char			*hex;
	char			*hexptr;
	int				printed;
	int				len;
	unsigned long	addr;

	if (address == NULL)
		addr = 0;
	else
		addr = (unsigned long)address;
	len = ft_lulen((unsigned long)addr, 16) + 2;
	hex = malloc((len + 1) * sizeof(char));
	if (!hex)
		return (-1);
	hexptr = hex;
	*hex++ = '0';
	*hex++ = 'x';
	ft_toptrlow(addr, hex);
	printed = ft_putstr(fd, hexptr);
	return (free(hexptr), printed);
}
