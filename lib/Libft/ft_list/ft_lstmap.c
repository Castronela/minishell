/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_lstmap.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pamatya <pamatya@student.42heilbronn.de    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/03/25 01:26:12 by pamatya           #+#    #+#             */
/*   Updated: 2024/03/25 01:26:12 by pamatya          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/libft.h"

t_list	*ft_lstmap(t_list *lst, void *(*f)(void *), void (*del)(void *))
{
	t_list	*new_lst;
	t_list	*node;
	t_list	*cont;

	if (!lst || !f || !del)
		return (NULL);
	new_lst = NULL;
	while (lst)
	{
		cont = f(lst->content);
		node = ft_lstnew(cont);
		ft_lstadd_back(&new_lst, node);
		if (!node || !new_lst)
			return (del(cont), ft_lstclear(&new_lst, del), NULL);
		lst = lst->next;
	}
	return (new_lst);
}
