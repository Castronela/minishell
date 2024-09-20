/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   lst_str_fns.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pamatya <pamatya@student.42heilbronn.de    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/09 00:52:33 by pamatya           #+#    #+#             */
/*   Updated: 2024/09/04 02:43:42 by pamatya          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/minishell.h"

t_lst_str	*ft_lst_new(char *str);
t_lst_str	*ft_lst_last(t_lst_str *list);
void		ft_lst_addback(t_lst_str **root, t_lst_str *new);
int			ft_lst_size(t_lst_str *root);
void		ft_lst_free(t_lst_str **root);

t_lst_str	*ft_lst_new(char *str)
{
	t_lst_str	*new_node;

	new_node = malloc(sizeof(t_lst_str));
	if (!new_node)
		return (NULL);
	new_node->str = ft_strdup(str);
	new_node->next = NULL;
	return (new_node);
}

t_lst_str	*ft_lst_last(t_lst_str *list)
{
	if (!list)
		return (NULL);
	while (list->next)
		list = list->next;
	return (list);
}

void	ft_lst_addback(t_lst_str **root, t_lst_str *new)
{
	t_lst_str	*last;

	if (!root || !new)
		return ;
	if (!*root)
	{
		*root = new;
		return ;
	}
	last = ft_lst_last(*root);
	last->next = new;
}

int	ft_lst_size(t_lst_str *root)
{
	int	i;

	if (!root)
		return (0);
	i = 1;
	while (root->next != NULL)
	{
		i++;
		root = root->next;
	}
	return (i);
}

void	ft_lst_free(t_lst_str **root)
{
	t_lst_str	*del_node;
	t_lst_str	*free_node;

	if (!root || !*root)
		return ;
	del_node = *root;
	while (del_node)
	{
		free_node = del_node;
		del_node = del_node->next;
		free(free_node->str);
		free(free_node);
	}
	*root = NULL;
}
