/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_lengths.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pamatya <pamatya@student.42heilbronn.de    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/03/31 20:27:23 by pamatya           #+#    #+#             */
/*   Updated: 2024/07/04 04:00:32 by pamatya          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/ft_printf.h"

int	ft_intlen(long num, int base)
{
	int	len;

	len = 0;
	if (num <= 0)
		len = 1;
	while (num)
	{
		len++;
		num /= base;
	}
	return (len);
}

int	ft_lulen(unsigned long num, int base)
{
	int	len;

	len = 0;
	if (num == 0)
		return (1);
	while (num)
	{
		len++;
		num /= base;
	}
	return (len);
}
