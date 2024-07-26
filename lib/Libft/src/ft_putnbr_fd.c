/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_putnbr_fd.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dstinghe <dstinghe@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/03/13 16:13:52 by dstinghe          #+#    #+#             */
/*   Updated: 2024/03/15 11:01:50 by dstinghe         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

void	ft_putnbr_fd(int n, int fd)
{
	long long	num;
	char		digit;

	num = (long long)n;
	if (num < 0)
	{
		write(fd, "-", 1);
		num = -num;
	}
	if (num < 10)
	{
		digit = '0' + (num % 10);
		write(fd, &digit, 1);
	}
	else
	{
		ft_putnbr_fd(num / 10, fd);
		digit = '0' + (num % 10);
		write(fd, &digit, 1);
	}
}

// int main()
// {
// 	int n = -5325;

// 	ft_putnbr_fd(n, 1);
// }