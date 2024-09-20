/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_free.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pamatya <pamatya@student.42heilbronn.de    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/06/21 22:19:54 by pamatya           #+#    #+#             */
/*   Updated: 2024/07/09 04:44:28 by pamatya          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/libft.h"

void	ft_free2d(char **memory)
{
	size_t	i;

	i = 0;
	while (memory[i] != NULL)
	{
		free(memory[i]);
		memory[i] = NULL;
		i++;
	}
	free(memory);
}

// Important:
// Note that the 1d pointer passed by reference here (**) should also exist by
// reference in the calling function. The correct way to call this function is
// to either call it from the function where the 1d pointer is dyn.ly allocated,
// or from another function which has also received the 1d pointer by reference
// (**) when the 1d pointer was allocated by another function which calls that
// function.
void	ft_free_safe(void **mem)
{
	char	**memory;

	memory = (char **)mem;
	if ((*memory) == NULL)
		return ;
	else
	{
		free(*memory);
		(*memory) = NULL;
	}
}

// Important:
// Note that the 2d pointer passed by reference here (***) should also exist by
// reference in the calling function. The correct way to call this function is
// to either call it from the function where the 2d pointer is dyn.ly allocated,
// or from another function which has also received the 2d pointer by reference
// (***) when the 2d pointer was allocated by another function which calls that
// function.
void	ft_free2d_safe(char ***memory)
{
	size_t	i;

	if ((*memory) == NULL)
		return ;
	i = 0;
	while ((*memory) && (*memory)[i] != NULL)
	{
		free((*memory)[i]);
		(*memory)[i] = NULL;
		i++;
	}
	if ((*memory) != NULL)
	{
		free((*memory));
		(*memory) = NULL;
	}
}
