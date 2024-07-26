/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_lstmap_bonus.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dstinghe <dstinghe@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/03/18 15:21:38 by dstinghe          #+#    #+#             */
/*   Updated: 2024/03/19 11:37:10 by dstinghe         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

t_list	*ft_lstmap(t_list *lst, void *(*f)(void *), void (*del)(void *))
{
	t_list	*result_lst;

	result_lst = NULL;
	if (!lst || !f || !del)
		return (NULL);
	while (lst)
	{
		if (lst->content)
		{
			ft_lstadd_back(&result_lst, ft_lstnew(f(lst->content)));
			if (!result_lst)
			{
				ft_lstclear(&result_lst, del);
				return (NULL);
			}
		}
		lst = lst->next;
	}
	return (result_lst);
}
