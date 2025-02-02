/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   lst_str_fns.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pamatya <pamatya@student.42heilbronn.de    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/09 00:52:33 by pamatya           #+#    #+#             */
/*   Updated: 2025/02/02 15:39:17 by pamatya          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/minishell.h"

t_lst_str	*ft_lst_new(char *key, char *val);
t_lst_str	*ft_lst_last(t_lst_str *list);
void		ft_lst_addback(t_lst_str **root, t_lst_str *new);
int			ft_lst_size(t_lst_str *root);
void		ft_lst_free(t_lst_str **root);

/*
Creates a new node of type t_lst_str for variable type list
  - Returns a new node of t_lst_str type struct
  - Mallocs memory for new node and uses ft_strdup to allocate memory for
  	args. key and val
  - Returns NULL if malloc fails
  - The fields ’next’ and 'prev' are initialized to NULL, used for creating
  	links
  - The fields 'key' and 'val' are initialized from values received as
  	parameters key and val
  - If any of the parameter is passed as NULL, the fields are also initialized
  	as NULL
*/
t_lst_str	*ft_lst_new(char *key, char *val)
{
	t_lst_str	*new_var;

	new_var = malloc(sizeof(t_lst_str));
	if (!new_var)
		return (NULL);
	new_var->key = NULL;
	new_var->val = NULL;
	if (key)
		new_var->key = ft_strdup(key);
	if (val)
		new_var->val = ft_strdup(val);
	new_var->next = NULL;
	new_var->prev = NULL;
	return (new_var);
}

/*
Finds the last node of the "t_lst_str" list
  - Returns the last node of the list of type t_lst_str
  - Returns NULL if the list is empty
Libft Description: Returns the last node of the list.
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
Libft Description: Adds the node ’new’ at the end of the list.
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
	new->prev = last;
}

/*
Determines the size of the "t_lst_str" list
  - Returns an integer value representing the number of nodes in the list
  - Returns 0 if "root" is NULL
Libft Description: Counts the number of nodes in a list.
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
		if (free_node->key)
			free(free_node->key);
		if (free_node->val)
			free(free_node->val);
		free_node->key = NULL;
		free_node->val = NULL;
		free(free_node);
		free_node = NULL;
	}
	*root = NULL;
}
