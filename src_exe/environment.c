/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   environment.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pamatya <pamatya@student.42heilbronn.de    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/25 13:55:41 by pamatya           #+#    #+#             */
/*   Updated: 2025/02/02 20:15:34 by pamatya          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/minishell.h"

void		add_to_environ(t_shell *shl, char *var, int append);
static void	add_append_ifvar_null(t_shell *shl, char *var, t_lst_str *lvar_node,
				int append);
static void	update_append_ifvar_exists(t_shell *shl, char *var, char **dp,
				int append);

/*
Function to add a env variable entry to shl->environ
  - if 'append' is 1, then the new value in 'var' is appended to the original
	variable
  - if 'append' is 0, a new entry is created when the variable does not already
	exist, if it does exist the value is updated to the new value of the 
	variable found in 'var'
*/
void	add_to_environ(t_shell *shl, char *var, int append)
{
	char		**dp;
	size_t		dp_len;
	t_lst_str	*lvar_node;
	char		*var_name;

	dp = find_string_addr(shl, var, var_offset(var, 0), 1);
	var_name = ft_substr(var, 0, var_offset(var, 0));
	if (!var_name)
		exit_early(shl, NULL, ERRMSG_MALLOC);
	lvar_node = ft_find_node(shl->local_vars, var_name, 0, 1);
	free(var_name);
	if (!dp && append == -1 && !lvar_node)
		return ;
	if (!dp)
	{
		dp_len = count_pointers(shl->environ);
		shl->environ = ft_recalloc(shl->environ, (dp_len + 2)
				* sizeof(*shl->environ), (dp_len + 1) * sizeof(*shl->environ));
		add_append_ifvar_null(shl, var, lvar_node, append);
		shl->environ[dp_len + 1] = NULL;
	}
	else
		update_append_ifvar_exists(shl, var, dp, append);
}

/*
Static helper fn for add_to_environ() fn, to add or append to shl->environ
when a variable entry doesn't already exist
*/
static void	add_append_ifvar_null(t_shell *shl, char *var, t_lst_str *lvar_node,
				int append)
{
	size_t	dp_len;
	char	*var_name;

	dp_len = count_pointers(shl->environ);
	var_name = NULL;
	if (append == -1 && lvar_node)
		shl->environ[dp_len] = concat_strings((const char *[])
			{lvar_node->key, "=", lvar_node->val, NULL});
	else if (append == 0)
		shl->environ[dp_len] = ft_strdup(var);
	else if (append == 1 && !lvar_node)
	{
		var_name = ft_substr(var, 0, var_offset(var, 0));
		if (!var_name)
			exit_early(shl, NULL, ERRMSG_MALLOC);
		shl->environ[dp_len] = concat_strings((const char *[]){var_name,
				"=", (var + var_offset(var, 1)), NULL});
		free(var_name);
	}
	else if (append == 1 && lvar_node)
		shl->environ[dp_len] = concat_strings((const char *[])
			{lvar_node->key, "=", lvar_node->val,
				(var + var_offset(var, 1)), NULL});
	if (!shl->environ[dp_len])
		exit_early(shl, NULL, ERRMSG_MALLOC);
}

/*
Static helper fn for add_to_environ() fn, to update or append to shl->environ
when a variable entry already exists
*/
static void	update_append_ifvar_exists(t_shell *shl, char *var, char **dp,
				int append)
{
	char	*new_var;

	new_var = NULL;
	if (append == -1)
		return ;
	if (append == 0)
		new_var = ft_strdup((var));
	else if (append == 1)
		new_var = concat_strings((const char *[]){*dp,
				(var + var_offset(var, 1)), NULL});
	if (!new_var)
		exit_early(shl, NULL, ERRMSG_MALLOC);
	free(*dp);
	*dp = new_var;
}
