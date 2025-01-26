/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   variables.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pamatya <pamatya@student.42heilbronn.de    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/25 13:39:39 by pamatya           #+#    #+#             */
/*   Updated: 2025/01/26 13:40:45 by pamatya          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/minishell.h"

void		store_as_variable(t_shell *shl, char *var, int append);

static int	get_new_varnode_ifno_var(t_lst_str **var_node, char *var_name,
				char *var_val, int append);
static int	update_append_ifvar_exists(t_lst_str *var_node, char *var,
				size_t offset, int append);

/*
Function to add a new varaible to the minishell memory but not to shl->env
  - The parameter 'var' is split against '=' and ported to ft_lst_new() fn
  - Only one variable is added per fn call
  - If malloc fails at any time, exit_early function is used to exit safely
  - The fn is called separately from mini_export() following the call to
	add_to_environ(), as it cannot itself add to shl->environ
  -	The fn does check in the local_vars list first to see if the variable entry
	already exists when an export call is made.
		- If yes, it deletes the local variable and creates an entry in
			shl->variables list with the updated value.
		- If no, then it simply creates an entry in shl->variables with the
			provided key and value.

Note:	var_node = ft_lst_new(*split, (var + offset)) is used instead of
		var_node = ft_lst_new(*split, *(split + 1)) ; for cases of arguments
		with multiple '=' character which would also be splitted by ft_split().
		eg.: abc=456=oiu54

!!! test the case for 'export abc+='
!!! test the case for 'export SHLVL' when SHLVL is already present
!!! test the case for 'export sth' when it not in env but is in local_vars with/wo some value
*/
void	store_as_variable(t_shell *shl, char *var, int append)
{
	t_lst_str	*var_node[2];
	char		*var_name;
	size_t		offset;

	offset = var_offset(var, 1);
	var_name = ft_substr(var, 0, var_offset(var, 0));
	if (!var_name)
		exit_early(shl, NULL, ERRMSG_MALLOC);
	var_node[0] = ft_find_node(shl->variables, var_name, 0, 1);
	var_node[1] = ft_find_node(shl->local_vars, var_name, 0, 1);
	if (var_node[0] == NULL)
	{
		if (get_new_varnode_ifno_var(var_node, var_name, (var + offset),
				append) < 0)
			exit_early2(shl, NULL, var_name, ERRMSG_MALLOC);
		ft_lst_addback(&shl->variables, var_node[0]);
		if (var_node[1])
			ft_remove_node(&shl->local_vars, &(var_node[1]));
	}
	else
		if (update_append_ifvar_exists(var_node[0], var, offset, append) < 0)
			exit_early2(shl, NULL, var_name, ERRMSG_MALLOC);
	free(var_name);
}

// Helper static function for store_as_variable() fn
static int	get_new_varnode_ifno_var(t_lst_str **var_node, char *var_name,
				char *var_val, int append)
{
	char	*new_val;

	new_val = NULL;
	if (append == -1)
	{
		if (var_node[1])
			var_node[0] = ft_lst_new(var_name, var_node[1]->val);
		else
			var_node[0] = ft_lst_new(var_name, NULL);
	}
	else if (append == 0 || (append == 1 && !var_node[1]))	// both cases of var_node[1] for append == 0 will be same
		var_node[0] = ft_lst_new(var_name, var_val);
	else if (append == 1 && var_node[1])
	{
		new_val = concat_strings((const char *[]){var_node[1]->val,
			var_val, NULL});
		if (!new_val)
			return (-1);
		var_node[0] = ft_lst_new(var_name, new_val);
		free(new_val);			
	}
	if (!var_node[0])
		return (-1);
	return (0);
}


// Helper static function for store_as_variable() fn
static int	update_append_ifvar_exists(t_lst_str *var_node, char *var,
				size_t offset, int append)
{
	char	*new_val;
	
	new_val = NULL;
	if (append == -1)
		return (0);
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

// void	store_as_variable(t_shell *shl, char *var, int append)
// {
// 	t_lst_str	*var_node[2];
// 	char		**split;
// 	size_t		offset;

// 	(void)append;
// 	offset = var_offset(var, 1);
// 	split = ft_split(var, '=');
// 	if (!split)
// 		exit_early(shl, NULL, ERRMSG_MALLOC);
// 	var_node[0] = ft_find_node(shl->variables, *split, 0, 1);
// 	var_node[1] = ft_find_node(shl->local_vars, *split, 0, 1);
// 	if (var_node[0] == NULL)
// 	{
// 		if (var_node[1])
// 			ft_remove_node(&shl->local_vars, &(var_node[1]));
// 		var_node[0] = ft_lst_new(*split, (var + offset));
// 		if (!var_node[0])
// 			exit_early(shl, split, ERRMSG_MALLOC);
// 		ft_lst_addback(&shl->variables, var_node[0]);
// 	}
// 	else if (var_node[0] && append == 0)
// 		if (update_var_val(var_node[0], var, offset) < 0)
// 			exit_early(shl, split, ERRMSG_MALLOC);
// 	ft_free2d(split);
// }

// // Helper static function for store_as_variable() fn
// static int	update_var_val(t_lst_str *var_node, char *var, size_t offset)
// {
// 	free(var_node->val);
// 	var_node->val = ft_strdup((var + offset));
// 	if (!var_node->val)
// 		return (-1);
// 	return (0);
// }