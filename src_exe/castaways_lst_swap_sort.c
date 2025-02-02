/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   castaways_lst_swap_sort.c                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pamatya <pamatya@student.42heilbronn.de    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/20 15:08:34 by pamatya           #+#    #+#             */
/*   Updated: 2025/02/02 17:52:58 by pamatya          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/minishell.h"

void		sort_lst_nodes(t_shell *shl, t_lst_str **root);
void		ft_swap_nodes(t_lst_str **root, t_lst_str *e1, t_lst_str *e2);
int			is_bash_reserved(char c);

static int	lst_count_nodes(t_lst_str *elm);
static void	get_indexed_arch_list(t_lst_str *list, t_lst_str **indices);

void	sort_lst_nodes(t_shell *shl, t_lst_str **root)
{
	t_lst_str	**indices;
	int			total_elements;
	int			i;

	total_elements = lst_count_nodes(*root);
	indices = malloc(total_elements * sizeof(t_lst_str *));
	if (!indices)
		exit_early(shl, NULL, ERRMSG_MALLOC);
	get_indexed_arch_list(*root, indices);
	while (--total_elements >= 0)
	{
		i = -1;
		while (++i < total_elements)
			if (ft_strcmp(indices[i]->key, indices[i + 1]->key) > 0)
				ft_swap_nodes(root, indices[i], indices[i + 1]);
	}
	free(indices);
}

// Function to count the total number of nodes in a linked list
static int	lst_count_nodes(t_lst_str *elm)
{
	int	i;

	if (!elm)
		return (0);
	i = 0;
	while (elm)
	{
		i++;
		elm = elm->next;
	}
	return (i);
}

// Function that populates the 'indices' array with pointers to each element of 
// the list 'list'
static void	get_indexed_arch_list(t_lst_str *list, t_lst_str **indices)
{
	int	i;

	i = 0;
	while (list)
	{
		*(indices + i) = list;
		i++;
		list = list->next;
	}
}

// Function to swap the position of two nodes in the linked list
void	ft_swap_nodes(t_lst_str **root, t_lst_str *e1, t_lst_str *e2)
{
	t_lst_str	*pen1[2];
	t_lst_str	*pen2[2];

	pen1[0] = e1->prev;
	pen1[1] = e1->next;
	pen2[0] = e2->prev;
	pen2[1] = e2->next;
	if (!e1->prev)
		*root = e2;
	else if (!e2->prev)
		*root = e1;
	if (pen1[0] && pen1[0] != e2)
		pen1[0]->next = e2;
	if (pen1[1] && pen1[1] != e2)
		pen1[1]->prev = e2;
	if (pen2[0] && pen2[0] != e1)
		pen2[0]->next = e1;
	if (pen2[1] && pen2[1] != e1)
		pen2[1]->prev = e1;
	e1->prev = e2->prev;
	e1->next = e2->next;
	e2->prev = pen1[0];
	e2->next = pen1[1];
}

/*
Function to check whether the char parameter is a bash-reserved character
  - Checks the character c within the array of special characters defined in the
	header file as bash-reserved characters using ft_strchr()
  - Returns 0 if the character is not found within the array
  - Returns 1 if the character is found within the array
*/
int	is_bash_reserved(char c)
{
	if (ft_strchr((const char []){BT, BN, DL, AD, SC, PO, PC, SQ, DQ, BS, PP,
			'\0'}, (unsigned int)c) == NULL)
		return (0);
	return (1);
}
