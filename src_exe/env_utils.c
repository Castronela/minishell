/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   env_utils.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pamatya <pamatya@student.42heilbronn.de    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/26 16:06:39 by pamatya           #+#    #+#             */
/*   Updated: 2025/02/02 13:15:17 by pamatya          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/minishell.h"

int			update_environ(char **var_ptr_addr, char *var_name, char *new_val);
char		*get_var_component(t_shell *shl, char *arg, int what);
void		update_env_var(t_shell *shl, t_cmds *cmd, char *var_name,
				char *val);

static void	update_var(t_shell *shl, t_cmds *cmd, char *var_name, char *val);

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
	if (update_environ(find_string_addr(shl, tmp_name, ft_strlen(tmp_name), 0),
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
