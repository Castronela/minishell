/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   lst_str_fns copy.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pamatya <pamatya@student.42heilbronn.de    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/09 00:52:33 by pamatya           #+#    #+#             */
/*   Updated: 2025/01/16 16:55:21 by pamatya          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/minishell.h"

t_lst_str	*ft_find_node(t_lst_str *list, char *str, int searchfield, int mod);
void		ft_replace_node(t_shell *shl, t_lst_str **old, t_lst_str *new);
void		ft_remove_node(t_lst_str **root, t_lst_str **node);
void		ft_del_node(t_lst_str **node);

/*
Function to search through the linked list using provided str and return
the pointer to that node, such that the manipulation of that node like adding-to
or its deletion is possible.
  - The list is of type t_lst_str so it has multiple fields, hence search-field
	argument is required to specify which field in the list to search
  - Search field can be 0 or 1, 0 meant for 'key' field and 1 meant for 'val'
	field to be searched
  - The toggle 'mod' can be either 0 or 1. 1 means exact search, and if mod is 0
	then it searches for the 'str' in the specified field of list even if the
	field is a larger string than 'str'. The function uses compare_strings to
	find the node so the 'mod' toggle is fed as the 'exact' parameter for that fn.
*/
t_lst_str	*ft_find_node(t_lst_str *list, char *str, int searchfield, int mod)
{
	if (!list || !str || searchfield < 0 || searchfield > 1)
		return (NULL);
	while (list)
	{
		if (searchfield == 0)
		{
			if (compare_strings(str, list->key, mod))
				return (/*printf("Found this zero:	%s\n", list->key), */list);
		}
		else if (searchfield == 1)
		{
			if (compare_strings(str, list->val, mod))
				return (/*printf("Found this one:	%s\n", list->val), */list);	
		}
		list = list->next;
	}
	return (NULL);
}

/*
Function to replace a node of the list with the new node and free the allocation
of the old node
  - Connects the next pointer of the old node's previous to the new node
  - Connects the new node's previous pointer to the prev of the old node
  - Connects the new node's next pointer to the next of the old node
  - Connects the previous pointer of the old node's next pointer to the new node

!!! Needs to be checked if this function needs to receive double pointers
-->>	Checked and corrected. Explanation below.
		Yes, a double pointer is needed. However, for the relinking part of this
		fn, the single pointer will do; the problem arises when ft_del_node is
		called. In this case, even though the address of the old pointer mayb be
		passed into the fn which expects a double pointer, this would be a diff
		address from the original pointer storing the node address. Therefore,
		for the mere sake of passing the pointer on by reference, this fn will
		also require to receive the original address of the old node.
*/
void	ft_replace_node(t_shell *shl, t_lst_str **old, t_lst_str *new)
{
	if (!(*old)->prev && !(*old)->next)		// Case 1: is first & last node
		shl->variables = new;
	else if (!(*old)->prev && (*old)->next)	// Case 2: first node of many
	{
		shl->variables = new;
		(*old)->next->prev = new;
		new->next = (*old)->next;
	}
	else if ((*old)->prev && !(*old)->next)	// Case 2: is last node of many
	{
		(*old)->prev->next = new;
		new->prev = (*old)->prev;
	}
	else									// Case 3: is middle node
	{
		(*old)->prev->next = new;
		(*old)->next->prev = new;
		new->prev = (*old)->prev;
		new->next = (*old)->next;
	}
	ft_del_node(old);
}

/*
Function to remove a certain node from the list and reconnect the severed pieces
  - If it is the first node, then it simply deletes the node and returns
  - If it is the last node, then it
  - Gets the nodes that are before and after it in the list
  - Connects the nodes before and after to each other
  - Deletes the node by freeing it using ft_del_node fn
*/
void	ft_remove_node(t_lst_str **root, t_lst_str **node)
{
	if (!root || !*root || !node)
		return ;
	if (!(*node)->prev && !(*node)->next)			// Case 1: is first & only node
		ft_lst_free(root);
	else
	{
		if (!(*node)->prev && (*node)->next)		// Case 2: first node of many
		{
			(*node)->next->prev = NULL;
			*root = (*node)->next;
		}
		else if ((*node)->prev && !(*node)->next)	// Case 2: is last node
			(*node)->prev->next = NULL;
		else										// Case 3: is middle node
		{
			(*node)->prev->next = (*node)->next;
			(*node)->next->prev = (*node)->prev;
		}
		ft_del_node(node);
	}
}

/*
Function to delete a single node in the list of t_lst_str type:
  - Frees the allocation of its malloc'd element (str)
  - Frees the allocation of the node itself
  
!!! Needs to be checked if this function needs to receive double pointer
-->>	Checked and corrected. Explanation below.
		Yes, a double pointer is required to set the original pointer holding 
		the old node address to NULL after freeing. Otherwise only the pointer
		that lives in the stack of this function which currently holds the old
		node address would be set to NULL instead, which will not reflect on the
		heap.
*/
void	ft_del_node(t_lst_str **node)
{
	// if (!node || !*node)
	if (!*node)
		return ;
	if ((*node)->key)
	{
		if ((*node)->key)
		{
			free((*node)->key);
			(*node)->key = NULL;
		}
		if ((*node)->val)
		{
			free((*node)->val);
			(*node)->val = NULL;
		}
	}
	free(*node);
	*node = NULL;
}
