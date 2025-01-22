/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   bi_export.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dstinghe <dstinghe@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/26 14:44:28 by pamatya           #+#    #+#             */
/*   Updated: 2025/01/20 18:56:06 by dstinghe         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/minishell.h"

int			mini_export(t_shell *shl, t_cmds *cmd);

static void	print_quoted_env(t_shell *shl);
static int	check_and_export_arg(t_shell *shl, char *arg, int *checks);
static int	is_valid_name(char *arg, int *i);
static int	is_valid_val(char *arg, int *i);

/*
Function for built-in export function
  - Adds the exported variable to the shl->env
  - Also adds the variable as key-val pair to the variables list

Note:	Here, the upd_str is only freed if checks[2] == 1 which indicates that
		it is a malloc'd string, otherwise it is simply a borrowed pointer
*/
int	mini_export(t_shell *shl, t_cmds *cmd)
{
	char	**arguments;
	int		checks[3];
	int		ret;
	char	*upd_str;

	upd_str = NULL;
	ret = 0;
	arguments = cmd->args + cmd->skip;
	if (*(arguments + 1) == NULL)
		return (print_quoted_env(shl), 0);
	while (*(++arguments))
	{
		upd_str = *arguments;
		if (check_and_export_arg(shl, *arguments, checks))
			cmd->exit_code = ERRCODE_GENERAL;
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
Function to print shl->environ with quotes around each value
*/
static void	print_quoted_env(t_shell *shl)
{
	char	**env;
	size_t	offst;

	env = shl->environ;
	while (*env)
	{
		offst = offset_to_env_value(*env);
		if (write(STDOUT_FILENO, "declare -x ", 11) < 0)
			exit_early(shl, NULL, ERRMSG_WRITE);
		if (write(STDOUT_FILENO, *env, offst) < 0)
			exit_early(shl, NULL, ERRMSG_WRITE);
		if (write(STDOUT_FILENO, "\"", 1) < 0)
			exit_early(shl, NULL, ERRMSG_WRITE);
		if (write(STDOUT_FILENO, (*env) + offst, ft_strlen((*env) + offst)) < 0)
			exit_early(shl, NULL, ERRMSG_WRITE);
		if (write(STDOUT_FILENO, "\"\n", 2) < 0)
			exit_early(shl, NULL, ERRMSG_WRITE);
		env++;
	}
}

/*
Static sub-function for export checks and execution if checks pass
*/
static int	check_and_export_arg(t_shell *shl, char *arg, int *checks)
{
	int	i;

	i = 0;
	checks[0] = is_valid_name(arg, &i);
	checks[1] = is_valid_val(arg, &i);
	checks[2] = 0;
	if (checks[0] < 0)
	{
		if ((ft_fprintf_str(STDERR_FILENO, (const char *[]){ERSHL, "export: `",
					arg, ERRMSG_NO_VALID_IDENT, NULL})) < 0)
			exit_early(shl, NULL, ERRMSG_WRITE);
		return (1);
	}
	if (checks[0] == 1 && checks[1] == 1)
	{
		add_to_environ(shl, arg);
		store_as_variable(shl, arg);
		checks[2] = 1;
	}
	if (checks[0] == 2)
		checks[2] = 1;
	return (0);
}

/*
Function to check whether variable name is valid
  - Checks if the name starts with alphabet or '_', and that the characters
	following that are alphanumeric
  - Returns 1 if the name is valid
  - Returns 0 if no '=' sign is found in the argument
  - Returns -n if the nth char in the var_name string is found to be invalid
  - Returns 2 if the variable name is just "_"
  - Used fns: ft_isalpha(), ft_isalnum()
*/
static int	is_valid_name(char *arg, int *i)
{
	if (arg[0] == '=')
		return (-1);
	if (!ft_isalpha(arg[0]) && arg[0] != '_')
		return (-1);
	if (arg[0] == '_' && arg[1] == '=')
		return (2);
	while (arg[++(*i)])
	{
		if (arg[*i] == '=')
			return (++(*i), 1);
		if (!ft_isalnum(arg[*i]) && arg[*i] != '_')
			return (-1 * (*i));
	}
	return (0);
}

/*
Function to check whether the variable value is a valid one
  - Checks if each character in the provided string is not a reserved bash
	character (is_bash_reserved), and that it is printable (ft_isprint)
  - Returns 0 if the variable value is found to be a reserved bash character
  - Returns -n when nth character in the argument is found to be non-printable
  - Returns 1 if the variable value is found to be valid
  - Used fns: is_bash_reserved(), ft_isprint()
*/
int	is_valid_val(char *arg, int *i)
{
	while (arg[*i])
	{
		// if (is_bash_reserved(arg[*i]))
		// 	return (0);
		if (!ft_isprint(arg[*i]))
			return (-1 * (*i));
		(*i)++;
	}
	return (1);
}
