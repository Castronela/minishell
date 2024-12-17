/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   lst_str_fns.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pamatya <pamatya@student.42heilbronn.de    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/09 00:52:33 by pamatya           #+#    #+#             */
/*   Updated: 2024/12/17 19:15:32 by pamatya          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/minishell.h"

t_lst_str	*ft_lst_new(char *key, char *val);
t_lst_str	*ft_lst_last(t_lst_str *list);
void		ft_lst_addback(t_lst_str **root, t_lst_str *new);
int			ft_lst_size(t_lst_str *root);
void		ft_lst_free(t_lst_str **root);
void		ft_replace_node(t_lst_str *old, t_lst_str *new);
void		ft_del_node(t_lst_str *node);
t_lst_str	*ft_find_node(t_lst_str *list, char *str, const int search_field);

/*
Creates a new node of type t_lst_str for variable type list
  - Returns a new node of t_lst_str type struct
  - Mallocs memory for new node and uses ft_strdup to allocate memory for args. key and val
  - Returns NULL if malloc fails
  - The fields ’next’ and 'prev' are initialized to NULL, used for creating links
  - The fields 'key' and 'val' are initialized from values received as parameters key and val
  - If any of the parameter is passed as NULL, the fields are also initialized as NULL
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
		free(free_node->key);
		free(free_node->val);
		free(free_node);
	}
	*root = NULL;
}

/*
Function to replace a node of the list with the new node and free the allocation
of the old node
  - Connects the next pointer of the old node's previous to the new node
  - Connects the new node's previous pointer to the prev of the old node
  - Connects the new node's next pointer to the next of the old node
  - Connects the previous pointer of the old node's next pointer to the new node

!!! Needs to be checked if this function needs to receive double pointers
*/
void	ft_replace_node(t_lst_str *old, t_lst_str *new)
{
	t_lst_str	*front;
	t_lst_str	*back;

	front = old->prev;
	back = old->next;
	front->next = new;
	new->prev = front;
	new->next = back;
	back->prev = new;
	ft_del_node(old);
}

/*
Function to delete a single node in the list of t_lst_str type:
  - Frees the allocation of its malloc'd element (str)
  - Frees the allocation of the node itself
  
!!! Needs to be checked if this function needs to receive double pointer
*/
void	ft_del_node(t_lst_str *node)
{
	// if (!node || !*node)
	if (!node)
		return ;
	if (node->key)
		free(node->key);
	if (node->val)
		free(node->val);
	free(node);
}

/*
Function to remove a certain node from the list and reconnect the severed pieces
  - If it is the first node, then it simply deletes the node and returns
  - If it is the last node, then it
  - Gets the nodes that are before and after it in the list
  - Connects the nodes before and after to each other
  - Deletes the node by freeing it using ft_del_node fn
*/
void	ft_remove_node(t_lst_str *node)
{
	t_lst_str	*before;
	t_lst_str	*after;

	if (!node->prev)
	{
		if (node->next)
			ft_del_node(node);
		return ;			// Have to confirm wheather this is enough or root pointer is required to be set to NULL, as it means that this is the first element
	}
	if (!node->next)
	{
		if (node->prev)
		{
			before = node->prev;
			before->next = NULL;	
		}
		ft_del_node(node);
		return ;
	}
	before = node->prev;
	after = node->next;
	before->next = after;
	after->prev = before;
	ft_del_node(node);
}

/*
Function to search through the linked list using provided str and return
the pointer to that node, such that the manipulation of that node like adding-to
or its deletion is possible.
  - The list is of type t_lst_str so it has multiple fields, hence search-field
	argument is required to specify which field in the list to search
  - Search field can be 0 or 1, 0 meant to 'key' field and 1 meant to search the
	'val' field
*/
t_lst_str	*ft_find_node(t_lst_str *list, char *str, const int search_field)
{
	if (!list || !str)
		return (NULL);
	if (search_field < 0 || search_field > 1)
		return (NULL);
	while (list)
	{
		if (search_field == 0)
		{
			if (compare_strings(str, list->key, 1))
				return (list);
		}
		else if (search_field == 1)
		{
			if (compare_strings(str, list->val, 1))
				return (list);	
		}
		list = list->next;
	}
	return (NULL);
}
