/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   env_utils_2.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pamatya <pamatya@student.42heilbronn.de    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/20 15:08:34 by pamatya           #+#    #+#             */
/*   Updated: 2025/01/26 13:40:45 by pamatya          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/minishell.h"

int			update_environ(char **var_ptr_addr, char *var_name, char *new_val);
void		store_local_variable(t_shell *shl, char *var);
char		*get_var_component(t_shell *shl, char *arg, int what);
void		sort_lst_nodes(t_shell *shl, t_lst_str **root);

static int	lst_count_nodes(t_lst_str *elm);
static void	get_indexed_arch_list(t_lst_str *list, t_lst_str **indices);
/*
Funciton to replace the string pointed to by 'var_ptr_addr' in shl->environ with
the new variable string malloc'd by joining 'var_name' and 'new_val', and free
the previous variable string.
TLDR: Updates the value of a variable entry i.e. char* in shl->environ
*/
int	update_environ(char **var_ptr_addr, char *var_name, char *new_val)
{
	char	*new;
	char	*tmp;

	if (var_ptr_addr == NULL)
		return (0);
	tmp = ft_strdup(new_val);
	if (!tmp)
		return (-1);
	new = ft_strjoin(var_name, tmp);
	free(tmp);
	if (!new)
		return (-1);
	free(*var_ptr_addr);
	*var_ptr_addr = new;
	return (0);
}

/*
Function to add a local varaible to the minishell memory but not as env-var
  - The parameter 'var' is split against '=' and ported to ft_lst_new() fn
  - Only one variable is added per fn call
  - If malloc fails at any time, exit_early function is used to exit safely
  - The fn is called from exec_var_assignments() fn

Note:	var_node = ft_lst_new(*split, (var + offset)) is used instead of
		var_node = ft_lst_new(*split, *(split + 1)) ; for cases of arguments
		with multiple '=' character which would also be splitted by ft_split().
		eg.: abc=456=oiu54
*/
void	store_local_variable(t_shell *shl, char *var)
{
	t_lst_str	*var_node;
	char		**split;
	size_t		offset;

	offset = var_offset(var, 1);
	split = ft_split(var, '=');
	if (!split)
		exit_early(shl, NULL, ERRMSG_MALLOC);
	var_node = ft_find_node(shl->local_vars, *split, 0, 1);
	if (var_node == NULL)
	{
		var_node = ft_lst_new(*split, (var + offset));
		if (!var_node)
			exit_early(shl, split, ERRMSG_MALLOC);
		ft_lst_addback(&shl->local_vars, var_node);
	}
	else
	{
		free(var_node->val);
		var_node->val = ft_strdup((var + offset));
		if (!var_node->val)
			exit_early(shl, split, ERRMSG_MALLOC);
	}
	ft_free2d(split);
}

/*
Function to get variable component
  - Returns the variable name if 'what' is 0
  - Returns the variable value if 'what' is 1
*/
char	*get_var_component(t_shell *shl, char *arg, int what)
{
	char	*var_comp;
	size_t	offset;

	if (what == 0)
	{
		offset = var_offset(arg, 0);
		var_comp = ft_substr(arg, 0, offset);
		if (!var_comp)
			exit_early(shl, NULL, ERRMSG_MALLOC);
	}
	else
	{
		offset = var_offset(arg, 1);
		var_comp = ft_substr(arg, offset, ft_strlen(arg + offset));
		if (!var_comp)
			exit_early(shl, NULL, ERRMSG_MALLOC);
	}
	return (var_comp);
}

// char	*get_var_component(t_shell *shl, char *arg, int what)
// {
// 	char	**split;
// 	char	*component;

// 	split = ft_split(arg, '=');
// 	component = NULL;
// 	if (!split)
// 		exit_early(shl, NULL, ERRMSG_MALLOC);
// 	if (what == 0)
// 	{
// 		component = ft_strdup(split[0]);
// 		if (!component)
// 			exit_early(shl, split, ERRMSG_MALLOC);
// 	}
// 	else if (what == 1)
// 	{
// 		component = ft_strdup(split[1]);
// 		if (!component)
// 			exit_early(shl, split, ERRMSG_MALLOC);
// 	}
// 	ft_free2d(split);
// 	return (component);
// }

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
