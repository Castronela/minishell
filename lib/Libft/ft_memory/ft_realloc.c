/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_realloc.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: david <david@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/06/20 15:03:40 by pamatya           #+#    #+#             */
/*   Updated: 2024/12/21 18:50:28 by david            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/libft.h"

// Function to mimic the realloc function utilizing the ft_memmove function
void	*ft_realloc(void *ptr, size_t size)
{
	unsigned char	*new_ptr;

	if (size == 0 && ptr)
		size = 1;
	new_ptr = malloc(size);
	if (!new_ptr)
		return (NULL);
	if (ptr)
		ft_memmove(new_ptr, ptr, size);
	free(ptr);
	ptr = new_ptr;
	return (ptr);
}

/*
Modified realloc function
	- allocates and nulls (\0) new string
	- if 'ptr' is double pointer, set 'old_size' to number of single pointers
	- if 'ptr' is type char*, set 'old_size' to 0
*/
void	*ft_recalloc(void *ptr, size_t size, size_t old_size)
{
	unsigned char	*new_ptr;

	if (size == 0 && ptr)
		size = 1;
	new_ptr = ft_calloc(size, sizeof(char));
	if (!new_ptr)
		return (NULL);
	if (old_size == 0)
		old_size = ft_strlen2(ptr);
	if (ptr)
		ft_memcpy(new_ptr, ptr, old_size);
	free(ptr);
	ptr = new_ptr;
	return (ptr);
}

// // Function to mimic the realloc function without utilizing ft_memmove
// void	*ft_realloc(void *ptr, size_t size)
// {
// 	unsigned char	*ptr_cpy;
// 	unsigned char	*new_ptr;
// 	unsigned char	*new_ptr_cpy;

// 	ptr_cpy = (unsigned char *)ptr;
// 	if (size == 0 && ptr)
// 		size = 1;
// 	new_ptr = malloc(size);
// 	if (!new_ptr)
// 		return (NULL);
// 	new_ptr_cpy = new_ptr;
// 	if (ptr)
// 	{
// 		while (size--)
// 			*new_ptr++ = *ptr_cpy++;
// 	}
// 	free(ptr);
// 	ptr = new_ptr_cpy;
// 	return (ptr);
// }
