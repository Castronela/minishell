/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_bzero.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pamatya <pamatya@student.42heilbronn.de    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/03/09 18:16:02 by pamatya           #+#    #+#             */
/*   Updated: 2024/06/13 12:40:48 by pamatya          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/libft.h"

void	*ft_bzero(void *str, size_t n)
{
	unsigned char	*new;

	if (n == 0)
		return (str);
	new = str;
	while (n--)
		*new++ = '\0';
	return (str);
}
