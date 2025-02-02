/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   variables_local.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pamatya <pamatya@student.42heilbronn.de    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/02 13:07:55 by pamatya           #+#    #+#             */
/*   Updated: 2025/02/02 17:54:26 by pamatya          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/minishell.h"

void		store_local_variable(t_shell *shl, char *var, int append);
static int	add_append_lvar_ifvar_exists(t_lst_str	*var_node, char *var,
				size_t offset, int append);

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
void	store_local_variable(t_shell *shl, char *var, int append)
{
	t_lst_str	*var_node;
	char		*var_name;
	size_t		offset;

	offset = var_offset(var, 1);
	var_name = ft_substr(var, 0, var_offset(var, 0));
	if (!var_name)
		exit_early(shl, NULL, ERRMSG_MALLOC);
	var_node = ft_find_node(shl->local_vars, var_name, 0, 1);
	if (var_node == NULL)
	{
		var_node = ft_lst_new(var_name, (var + offset));
		if (!var_node)
			exit_early2(shl, NULL, var_name, ERRMSG_MALLOC);
		ft_lst_addback(&shl->local_vars, var_node);
	}
	else
		if (add_append_lvar_ifvar_exists(var_node, var, offset, append) == -1)
			exit_early2(shl, NULL, var_name, ERRMSG_MALLOC);
	free(var_name);
}

// Helper static fn for store_local_variable() fn
static int	add_append_lvar_ifvar_exists(t_lst_str	*var_node, char *var,
				size_t offset, int append)
{
	char	*new_val;

	new_val = NULL;
	if (append == 0)
		new_val = ft_strdup((var + offset));
	else if (append == 1)
		new_val = concat_strings((const char *[]){var_node->val,
				(var + offset), NULL});
	if (!new_val)
		return (-1);
	free(var_node->val);
	var_node->val = new_val;
	return (0);
}
