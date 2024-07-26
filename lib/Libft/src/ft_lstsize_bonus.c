/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_lstsize_bonus.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dstinghe <dstinghe@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/03/18 11:56:40 by dstinghe          #+#    #+#             */
/*   Updated: 2024/03/19 11:37:22 by dstinghe         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

int	ft_lstsize(t_list *lst)
{
	int		node_count;
	t_list	*ptr;

	ptr = lst;
	node_count = 0;
	while (ptr)
	{
		node_count++;
		ptr = ptr->next;
	}
	return (node_count);
}

// #include <stdio.h>
// int main()
// {
// 	t_list *root, *elem1, *elem2;
// 	root = malloc(sizeof(t_list));
// 	elem1 = malloc(sizeof(t_list));
// 	elem2 = malloc(sizeof(t_list));
// 	root->next = elem1;
// 	elem1->next = elem2;
// 	elem2->next = NULL;

// 	printf("%d", ft_lstsize(root));
// }