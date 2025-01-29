/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   bi_unset.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: david <david@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/26 14:44:43 by pamatya           #+#    #+#             */
/*   Updated: 2025/01/29 01:31:34 by david            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/minishell.h"

int			mini_unset(t_shell *shl, t_cmds *cmd);

static void	check_and_unset_arg(t_shell *shl, t_cmds *cmd, char *arg, 
			int *checks);
static int	is_valid_unset_name(char *arg);
static void	remove_from_environ(t_shell *shl, char *var_name);
static void	remove_variable(t_shell *shl, char *arg);

/*
Function that mimics the built-in unset command in shell

Note:	Here, the upd_str is only freed if checks[2] == 1 which indicates that
		it is a malloc'd string, otherwise it is simply a borrowed pointer

!!! Needs to be checked for cases when the return code is not zero and also
	figure out how to handle returning that.
*/
int	mini_unset(t_shell *shl, t_cmds *cmd)
{
	char	**arguments;
	int		checks[3];
	int		ret;
	char	*upd_str;

	upd_str = NULL;
	ret = 0;
	arguments = cmd->args + cmd->skip;
	if (*(arguments + 1) == NULL)
		return (0);
	while (*(++arguments))
	{
		upd_str = *arguments;
		check_and_unset_arg(shl, cmd, *arguments, checks);
		if (*(arguments + 1) == NULL && checks[2] == 1)
			upd_str = get_var_component(shl, *arguments, 0);
		if (checks[0] < 1)
			ret = 1;
	}
	update_env_var(shl, cmd, UNDERSCORE, upd_str);
	if (upd_str && checks[2] == 1)
		free(upd_str);
	return (ret);
}

/*
Static sub-function for export checks, and execution if checks pass
*/
static void	check_and_unset_arg(t_shell *shl, t_cmds *cmd, char *arg, 
	int *checks)
{
	checks[0] = is_valid_unset_name(arg);
	checks[1] = 0;
	checks[2] = 0;
	if (checks[0] < 0)
	{
		if ((ft_fprintf_str(STDERR_FILENO, (const char *[]){ERSHL, "unset: `",
					arg, ERRMSG_NO_VALID_IDENT, NULL})) < 0)
			exit_early(shl, NULL, ERRMSG_WRITE);
		cmd->exit_code = ERRCODE_GENERAL;
	}
	if (checks[0] == 1)
	{
		remove_from_environ(shl, arg);
		remove_variable(shl, arg);
		checks[2] = 1;
	}
	if (checks[0] == 2)
		checks[2] = 1;
}

static int	is_valid_unset_name(char *arg)
{
	int	i;

	i = 0;
	if (arg[0] == '=')
		return (-1);
	if (!ft_isalpha(arg[0]) && arg[0] != '_')
		return (-1);
	if (arg[0] == '_' && arg[1] == '\0')
		return (2);
	while (arg[++i])
	{
		if (arg[i] == '=')
			return (++i, -1);
		if (!ft_isalnum(arg[i]) && arg[i] != '_')
			return (-1 * i);
	}
	return (1);
}

/*
Function to remove a variable from environment
*/
static void	remove_from_environ(t_shell *shl, char *var_name)
{
	int		dp_index;
	size_t	dp_len;
	size_t	var_len;
	char	**envar;

	dp_len = count_pointers(shl->environ);
	envar = shl->environ;
	var_len = var_offset(var_name, 1);
	dp_index = find_dptr_index(shl, var_name, var_len);
	if (dp_index >= 0)
	{
		free(envar[dp_index]);
		while (envar[dp_index + 1])
		{
			*(envar + dp_index) = *(envar + dp_index + 1);
			dp_index++;
		}
		shl->environ = ft_recalloc(shl->environ, dp_len * sizeof(*shl->environ),
				(dp_len + 1) * sizeof(*shl->environ));
		shl->environ[dp_len - 1] = NULL;
	}
}

static void	remove_variable(t_shell *shl, char *arg)
{
	t_lst_str	*del_var;
	t_lst_str	*del_local_var;

	del_var = ft_find_node(shl->variables, arg, 0, 1);
	if (del_var)
		ft_remove_node(&shl->variables, &del_var);
	del_local_var = ft_find_node(shl->local_vars, arg, 0, 1);
	if (del_local_var)
		ft_remove_node(&shl->local_vars, &del_local_var);
}
