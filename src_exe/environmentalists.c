/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   environmentalists.c                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pamatya <pamatya@student.42heilbronn.de    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/26 16:06:39 by pamatya           #+#    #+#             */
/*   Updated: 2025/01/06 19:15:01 by pamatya          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/minishell.h"

void	update_env_var(t_shell *shl, t_cmds *cmd, char *var_name, char *val);
void	store_as_variable(t_shell *shl, char *var);

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
	char		*tmp;
	char		*var_val;
	t_lst_str	*env_lst[2];

	tmp = ft_strjoin(var_name, "=");
	if (!tmp)
		exit_early(shl, NULL, ERRMSG_MALLOC);
	if (update_environ(find_string_ptr(shl, tmp, ft_strlen(tmp)), tmp,
			cmd->bin_path) == -1)
	{
		free(tmp);
		exit_early(shl, NULL, ERRMSG_MALLOC);
	}
	free(tmp);
	if (val == NULL)
		var_val = cmd->args[count_pointers(cmd->args) - 1];
	else
		var_val = val;
	env_lst[0] = ft_find_node(shl->variables, var_name, 0, 1);
	env_lst[1] = ft_lst_new(var_name, var_val);
	ft_replace_node(shl, &(env_lst[0]), env_lst[1]);
}

/*
Function to add a new varaible to the minishell memory but not to shl->env
  - The parameter 'str' is split against '=' and ported intoft_lst_new fn
  - One variable is added upon per fn call
  - Does not write the variable to shl->env, this is only possible using the
    mini_export function
  - If malloc fails at any time, exit_early function is used to exit safely

Note:	var_node = ft_lst_new(*split, (var + var_len)) is used instead of
		var_node = ft_lst_new(*split, *(split + 1)) ; for cases of arguments
		with multiple '=' character which would also be splitted by ft_split().
		eg.: abc=456=oiu54
*/
void	store_as_variable(t_shell *shl, char *var)
{
	t_lst_str	*var_node;
	char		**split;
	size_t		var_len;

	var_len = var_name_length(var);
	split = ft_split(var, '=');
	if (!split)
		exit_early(shl, NULL, ERRMSG_MALLOC);
	var_node = ft_find_node(shl->variables, *split, 0, 1);
	if (var_node == NULL)
	{
		var_node = ft_lst_new(*split, (var + var_len));
		if (!var_node)
			exit_early(shl, split, "Could not malloc new variable list node");
		ft_lst_addback(&shl->variables, var_node);
	}
	else
	{
		free(var_node->val);
		var_node->val = ft_strdup((var + var_len));
		if (!var_node->val)
			exit_early(shl, split, ERRMSG_MALLOC);
	}
	ft_free2d(split);
}
