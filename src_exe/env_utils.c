/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   env_utils.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: david <david@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/26 16:06:39 by pamatya           #+#    #+#             */
/*   Updated: 2025/01/22 05:34:53 by david            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/minishell.h"

void		update_env_var(t_shell *shl, t_cmds *cmd, char *var_name,
				char *val);
void		add_to_environ(t_shell *shl, char *var);
void		store_as_variable(t_shell *shl, char *var);

static void	update_var(t_shell *shl, t_cmds *cmd, char *var_name, char *val);
static int	update_var_val(t_lst_str *var_node, char *var, size_t offset);

/*
Function to update a variable in both shl->environ and shl->variables
  - cmd is the command that invokes the fn call
  -	var_name is the name of the variable to update
  -	'val' is the value of the variable to update to; if it is set to NULL, it is
	the set to the last argument of the previous command. This option is for
	generalizing the fn and making it more able to taylor to each calling fn.
  -	If there is a custom value that the variables list needs to be updated to,
	then it should be specified with this argument 'val.
  -	Setting the 'val' argument only affects shl->variables. The value of the
	variable in shl->environ will still be updated to the cmd->bin_path.
  -	This function is made to be invoked upon the execution of every cmd in the
	case of updating $_ variable.
*/
void	update_env_var(t_shell *shl, t_cmds *cmd, char *var_name, char *val)
{
	char	*tmp_name;
	char	*var_val;

	tmp_name = ft_strjoin(var_name, "=");
	if (!tmp_name)
		exit_early(shl, NULL, ERRMSG_MALLOC);
	if (compare_strings(tmp_name, "_=", 1))
	{
		if (cmd->bin_path)
			var_val = cmd->bin_path;
		else
			var_val = cmd->args[count_pointers(cmd->args) - 1];
	}
	else
		var_val = val;
	if (update_environ(find_string_addr(shl, tmp_name, ft_strlen(tmp_name)),
			tmp_name, var_val) == -1)
	{
		free(tmp_name);
		exit_early(shl, NULL, ERRMSG_MALLOC);
	}
	free(tmp_name);
	update_var(shl, cmd, var_name, val);
}

// Static helper function for update_env_var() fn
static void	update_var(t_shell *shl, t_cmds *cmd, char *var_name, char *val)
{
	t_lst_str	*env_lst[2];
	char		*var_val;

	if (val == NULL)
		var_val = cmd->args[count_pointers(cmd->args) - 1];
	else
		var_val = val;
	env_lst[0] = ft_find_node(shl->variables, var_name, 0, 1);
	if (env_lst[0])
	{
		env_lst[1] = ft_lst_new(var_name, var_val);
		ft_replace_node(shl, &(env_lst[0]), env_lst[1]);
	}
}

// void	update_env_var(t_shell *shl, t_cmds *cmd, char *var_name, char *val)
// {
// 	char		*tmp_name;
// 	char		*var_val;
// 	t_lst_str	*env_lst[2];

// 	tmp_name = ft_strjoin(var_name, "=");
// 	if (!tmp_name)
// 		exit_early(shl, NULL, ERRMSG_MALLOC);
// 	if (compare_strings(tmp_name, "_=", 1))
// 	{
// 		if (cmd->bin_path)
// 			var_val = cmd->bin_path;
// 		else
// 			var_val = cmd->args[count_pointers(cmd->args) -1];
// 	}
// 	else
// 		var_val = val;
// 	if (update_environ(find_string_addr(shl, tmp_name, ft_strlen(tmp_name)),
// 			tmp_name, var_val) == -1)
// 	{
// 		free(tmp_name);
// 		exit_early(shl, NULL, ERRMSG_MALLOC);
// 	}
// 	free(tmp_name);
// 	if (val == NULL)
// 		var_val = cmd->args[count_pointers(cmd->args) - 1];
// 	else
// 		var_val = val;
// 	env_lst[0] = ft_find_node(shl->variables, var_name, 0, 1);
// 	env_lst[1] = ft_lst_new(var_name, var_val);
// 	if (env_lst[0])
// 		ft_replace_node(shl, &(env_lst[0]), env_lst[1]);
// }

/*
Function to add a env variable entry to shl->environ
*/
void	add_to_environ(t_shell *shl, char *var)
{
	char	**dp;
	size_t	dp_len;
	size_t	var_len;

	var_len = offset_to_env_value(var);
	dp = find_string_addr(shl, var, var_len);
	if (dp == NULL)
	{
		dp_len = count_pointers(shl->environ);
		shl->environ = ft_recalloc(shl->environ, (dp_len + 2)
				* sizeof(*shl->environ), (dp_len + 1) * sizeof(*shl->environ));
		shl->environ[dp_len] = ft_strdup(var);
		if (!shl->environ[dp_len])
			exit_early(shl, NULL, ERRMSG_MALLOC);
		shl->environ[dp_len + 1] = NULL;
	}
	else
	{
		free(*dp);
		*dp = ft_strdup((var));
		if (!*dp)
			exit_early(shl, NULL, ERRMSG_MALLOC);
	}
}

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
*/
void	store_as_variable(t_shell *shl, char *var)
{
	t_lst_str	*var_node[2];
	char		**split;
	size_t		offset;

	offset = offset_to_env_value(var);
	split = ft_split(var, '=');
	if (!split)
		exit_early(shl, NULL, ERRMSG_MALLOC);
	var_node[0] = ft_find_node(shl->variables, *split, 0, 1);
	var_node[1] = ft_find_node(shl->local_vars, *split, 0, 1);
	if (var_node[0] == NULL)
	{
		if (var_node[1])
			ft_remove_node(&shl->local_vars, &(var_node[1]));
		var_node[0] = ft_lst_new(*split, (var + offset));
		if (!var_node[0])
			exit_early(shl, split, ERRMSG_MALLOC);
		ft_lst_addback(&shl->variables, var_node[0]);
	}
	else if (var_node[0])
		if (update_var_val(var_node[0], var, offset) < 0)
			exit_early(shl, split, ERRMSG_MALLOC);
	ft_free2d(split);
}

// Helper static function for store_as_variable() fn
static int	update_var_val(t_lst_str *var_node, char *var, size_t offset)
{
	free(var_node->val);
	var_node->val = ft_strdup((var + offset));
	if (!var_node->val)
		return (-1);
	return (0);
}

// void	store_as_variable(t_shell *shl, char *var)
// {
// 	t_lst_str	*var_node[2];
// 	char		**split;
// 	size_t		offset;

// 	offset = offset_to_env_value(var);
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
// 	else if (var_node[0])
// 	{
// 		free(var_node[0]->val);
// 		var_node[0]->val = ft_strdup((var + offset));
// 		if (!var_node[0]->val)
// 			exit_early(shl, split, ERRMSG_MALLOC);
// 	}
// 	ft_free2d(split);
// }
