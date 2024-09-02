/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_atoi.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pamatya <pamatya@student.42heilbronn.de    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/03/12 20:51:28 by pamatya           #+#    #+#             */
/*   Updated: 2024/06/12 23:47:35 by pamatya          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/libft.h"

int	ft_atoi(const char *str)
{
	int	signe;
	int	num;
	int	i;
	int	s_count;

	signe = 1;
	num = 0;
	i = 0;
	s_count = 0;
	while (str[i] == ' ' || str[i] == '\n' || str[i] == '\t'
		|| str[i] == '\v' || str[i] == '\f' || str[i] == '\r')
		i ++;
	while (str[i] == '+' || str[i] == '-')
	{
		s_count++;
		if (str[i] == '-')
			signe *= -1;
		i++;
	}
	if (s_count > 1)
		return (0);
	while (str[i] >= '0' && str[i] <= '9')
		num = num * 10 + (str[i++] - '0');
	num = signe * num;
	return (num);
}
