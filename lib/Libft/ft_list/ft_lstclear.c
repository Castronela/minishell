/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_lstclear.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pamatya <pamatya@student.42heilbronn.de    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/03/25 01:04:26 by pamatya           #+#    #+#             */
/*   Updated: 2024/03/25 01:04:26 by pamatya          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/libft.h"

void	ft_lstclear(t_list **lst, void (*del)(void *))
{
	t_list	*del_node;
	t_list	*free_node;

	if (!lst || !del)
		return ;
	del_node = *lst;
	while (del_node)
	{
		free_node = del_node;
		del(del_node->content);
		del_node = del_node->next;
		free(free_node);
	}
	*lst = NULL;
}
