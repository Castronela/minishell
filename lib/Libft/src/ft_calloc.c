/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_calloc.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dstinghe <dstinghe@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/03/11 15:55:03 by dstinghe          #+#    #+#             */
/*   Updated: 2024/03/16 14:26:36 by dstinghe         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

static void	my_bzero(void *s, size_t n)
{
	char	*ss;

	ss = s;
	while (n > 0)
	{
		*ss = '\0';
		ss++;
		n--;
	}
}

void	*ft_calloc(size_t count, size_t size)
{
	void	*alloc_memory;
	int		tot_size;

	tot_size = count * size;
	alloc_memory = malloc(tot_size);
	if (alloc_memory == NULL)
		return (NULL);
	if (alloc_memory)
		my_bzero(alloc_memory, tot_size);
	return (alloc_memory);
}
