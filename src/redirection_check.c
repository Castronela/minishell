/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   redirection_check.c                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: castronela <castronela@student.42.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/29 11:48:27 by castronela        #+#    #+#             */
/*   Updated: 2024/09/29 17:22:44 by castronela       ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static int			check_access(t_lst_str *node_target);
static t_lst_str	*next_command_node(t_lst_str *node);

/*
(Main) Verifies validity of all redirection targets from 'head' linked list;
On failure: returns 1 (nothing to free);
Caution: should not exit program!
*/
int	redirection_check(t_lst_str *head)
{
	int	bad_access;

	bad_access = 0;
	while (head)
	{
		while (head && !is_op(head->str, CT))
		{
			if (is_op(head->str, RD))
			{
				if (check_access(head->next))
				{
					bad_access = 1;
					break ;
				}
			}
			head = head->next;
		}
		head = next_command_node(head);
	}
	return (bad_access);
}

/*
Verifies presence and access rights of redirection target,
	based on redirection type;
If redirection is outbound and file within valid directory path is not present,
will attempt creating new file with access rights from macro	DEFAULT_ACCESS;
On failure: prints error, returns 1;
*/
static int	check_access(t_lst_str *node_target)
{
	int	access_flag;
	int	fd;
	int	ret_no;

	if (!ft_strncmp(node_target->prev->str, "<", 2))
		access_flag = R_OK;
	else
		access_flag = W_OK;
	ret_no = 0;
	if (access_flag == W_OK && access(node_target->str, F_OK))
	{
		fd = open(node_target->str, O_CREAT, DEFAULT_ACCESS);
		if (fd == -1)
		{
			printf("minishell: %s: %s\n", node_target->str, strerror(errno));
			ret_no = 1;
		}
		close(fd);
	}
	else if (access(node_target->str, access_flag))
	{
		printf("minishell: %s: %s\n", node_target->str, strerror(errno));
		ret_no = 1;
	}
	return (ret_no);
}

/*
Returns next node after control operator or NULL if none encountered
*/
static t_lst_str	*next_command_node(t_lst_str *node)
{
	while (node)
	{
		if (is_op(node->str, CT))
			return (node->next);
		node = node->next;
	}
	return (node);
}
