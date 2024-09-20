/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   lst_str_fns.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pamatya <pamatya@student.42heilbronn.de    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/09 00:52:33 by pamatya           #+#    #+#             */
/*   Updated: 2024/09/18 17:45:53 by pamatya          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/minishell.h"

t_lst_str	*ft_lst_new(char *str);
t_lst_str	*ft_lst_last(t_lst_str *list);
void		ft_lst_addback(t_lst_str **root, t_lst_str *new);
int			ft_lst_size(t_lst_str *root);
void		ft_lst_free(t_lst_str **root);

/*
Creates a new node of type t_lst_str
  - Returns a new node of t_lst_str type struct
  - Mallocs memory for new node and uses ft_strdup to allocate memory for arg. "str"
  - Returns NULL if malloc fails
*/
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

/*
Finds the last node of the "t_lst_str" list
  - Returns the last node of the list of type t_lst_str
  - Returns NULL if the list is empty
*/
t_lst_str	*ft_lst_last(t_lst_str *list)
{
	if (!list)
		return (NULL);
	while (list->next)
		list = list->next;
	return (list);
}

/*
Adds a new node to the end of the "t_lst_str" list
  - Adds the "new" node to the end of the list pointed to by "root"
  - If the list is empty, the root is updated to point to the new node
  - Returns the control to the calling function without doing anything if the 
	"root" or "new" is NULL
*/
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

/*
Determines the size of the "t_lst_str" list
  - Returns an integer value representing the number of nodes in the list
  - Returns 0 if "root" is NULL
*/
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

/*
Frees the memory allocated for the list of type "t_lst_str"
  - Frees the memory allocated for the list of type t_lst_str
  - Also frees the memory allocated for the string in each node
  - Sets the *root to NULL after freeing the memory
  - Simply returns control if root or *root is NULL
*/
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
