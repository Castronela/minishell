/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_lstclear_bonus.c                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dstinghe <dstinghe@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/03/18 14:26:11 by dstinghe          #+#    #+#             */
/*   Updated: 2024/03/19 11:36:47 by dstinghe         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

void	ft_lstclear(t_list **lst, void (*del)(void *))
{
	t_list	*node_index;
	t_list	*node_free;

	if (!lst || !del)
		return ;
	node_index = *lst;
	node_free = *lst;
	while (node_index)
	{
		node_index = node_index->next;
		del(node_free->content);
		free(node_free);
		node_free = node_index;
	}
	*lst = NULL;
}
