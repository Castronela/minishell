/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   bi_export.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pamatya <pamatya@student.42heilbronn.de    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/26 14:44:28 by pamatya           #+#    #+#             */
/*   Updated: 2025/01/09 15:06:37 by pamatya          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/minishell.h"

int			mini_export(t_shell *shl, t_cmds *cmd);
static void	check_and_export_arg(t_shell *shl, char *arg, int *checks);
static void	print_quoted_env(t_shell *shl);
void		add_to_environ(t_shell *shl, char *var);
static int	is_valid_name(char *arg, int *i);
static int	is_valid_val(char *arg, int *i);
int			is_bash_reserved(char c);
char		*get_var_component(t_shell *shl, char *arg, int what);

/*
Function for built-in export function
  - Adds the exported variable to the shl->env
  - Also adds the variable as key-val pair to the variables list

Note:	Here, the upd_str is only freed if checks[2] == 1 which indicates that
		it is a malloc'd string, otherwise it is simply a borrowed pointer

!!! This fn doesn't yet store env variables in the shl->environ (char **)
-->>	Now it does.

!!! Consider adding functionality for aliases with a simple file or just by
	adding a list

!!! Use the new ft_fprintf_str() fn instead of multiples writes here.
*/

// int	mini_export_old(t_shell *shl, t_cmds *cmd)
// {
// 	int		i;
// 	char	**arguments;
// 	int		checks[3];
// 	char	*upd_str;

// 	arguments = cmd->args;
// 	while (*(++arguments))
// 	{
// 		upd_str = *arguments;
// 		i = 0;
// 		checks[2] = 0;
// 		checks[0] = is_valid_name(*arguments, &i);
// 		checks[1] = is_valid_val(*arguments, &i);
// 		if (checks[0] < 0)
// 			ft_fprintf_str(STDERR_FILENO, (const char *[]){"minishell: ", 
// 			"export: `", *arguments, "\': not a valid identifier\n", NULL});
// 		if (checks[0] == 1 && checks[1] == 1)
// 		{
// 			checks[2] = 1;
// 			add_to_environ(shl, *arguments);
// 			store_as_variable(shl, *arguments);
// 		}
// 		if (checks[0] == 2)
// 			checks[2] = 1;
// 		if (*(arguments + 1) == NULL && checks[2] == 1)
// 			upd_str = get_var_component(shl, *arguments, 0);
// 	}
// 	update_env_var(shl, cmd, UNDERSCORE, upd_str);
// 	if (checks[2] == 1)
// 		free(upd_str);
// 	return (0);
// }

int	mini_export(t_shell *shl, t_cmds *cmd)
{
	char	**arguments;
	int		checks[3];
	int		ret;
	char	*upd_str;

	upd_str = NULL;
	ret = 0;
	arguments = cmd->args;
	if (*(cmd->args + 1) == NULL)
		return (print_quoted_env(shl), 0);
	while (*(++arguments))
	{
		upd_str = *arguments;
		check_and_export_arg(shl, *arguments, checks);
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
static void	check_and_export_arg(t_shell *shl, char *arg, int *checks)
{
	int	i;

	i = 0;
	checks[0] = is_valid_name(arg, &i);
	checks[1] = is_valid_val(arg, &i);
	checks[2] = 0;
	if (checks[0] < 0)
	{
		if ((ft_fprintf_str(STDERR_FILENO, (const char *[]){"minishell: ", 
				"export: `", arg, "\': not a valid identifier\n", NULL})) < 0)
			exit_early(shl, NULL, ERRMSG_WRITE);
	}
	if (checks[0] == 1 && checks[1] == 1)
	{
		add_to_environ(shl, arg);
		store_as_variable(shl, arg);
		checks[2] = 1;
	}
	if (checks[0] == 2)
		checks[2] = 1;
}

/*
Origin file: bi_export.c
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
		shl->environ = ft_recalloc(shl->environ, (dp_len + 2) * 
				sizeof(*shl->environ), (dp_len + 1) * sizeof(*shl->environ));
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
		if (is_bash_reserved(arg[*i]))
			return (0);
		if (!ft_isprint(arg[*i]))
			return (-1 * (*i));
		(*i)++;
	}
	return (1);
}

/*
Function to check whether the char parameter is a bash-reserved character
  - Checks the character c within the array of special characters defined in the
	header file as bash-reserved characters using ft_strchr()
  - Returns 0 if the character is not found within the array
  - Returns 1 if the character is found within the array
*/
int	is_bash_reserved(char c)
{
	if (ft_strchr((const char []){RESERVED_BASH_CHARS, '\0'}, 
			(unsigned int)c) == NULL)
		return (0);
	return (1);
}

/*
Function to get variable component
  - Returns the variable name if 'what' is 0
  - Returns the variable value if 'what' is 1
*/
char	*get_var_component(t_shell *shl, char *arg, int what)
{
	char	**split;
	char	*component;

	split = ft_split(arg, '=');
	component = NULL;
	if (!split)
		exit_early(shl, NULL, ERRMSG_MALLOC);
	if (what == 0)
	{
		component = ft_strdup(split[0]);
		if (!component)
			exit_early(shl, split, ERRMSG_MALLOC);
	}
	else if (what == 1)
	{
		component = ft_strdup(split[1]);
		if (!component)
			exit_early(shl, split, ERRMSG_MALLOC);
	}
	ft_free2d(split);
	return (component);
}





// int	main(int ac, char **av, char **envp)
// {
// 	char	a;
// 	char	b;
// 	int		i;
// 	int		j;

// 	(void)ac;
// 	(void)av;
// 	(void)envp;

// 	a = 'l';
// 	b = '$';
// 	i = is_bash_reserved(a);
// 	j = is_bash_reserved(b);
	
// 	printf("a = %c	and i = %d\n", a, i);
// 	printf("b = %c	and i = %d\n\n\n", b, j);
	
// 	b = '`';
// 	j = is_bash_reserved(b);
// 	printf("b = %c	and is_bash_reserved gives %d\n", b, j);
	
// 	b = '!';
// 	j = is_bash_reserved(b);
// 	printf("b = %c	and is_bash_reserved gives %d\n", b, j);
	
// 	b = '$';
// 	j = is_bash_reserved(b);
// 	printf("b = %c	and is_bash_reserved gives %d\n", b, j);
	
// 	b = '&';
// 	j = is_bash_reserved(b);
// 	printf("b = %c	and is_bash_reserved gives %d\n", b, j);
	
// 	b = ';';
// 	j = is_bash_reserved(b);
// 	printf("b = %c	and is_bash_reserved gives %d\n", b, j);
	
// 	b = '(';
// 	j = is_bash_reserved(b);
// 	printf("b = %c	and is_bash_reserved gives %d\n", b, j);
	
// 	b = ')';
// 	j = is_bash_reserved(b);
// 	printf("b = %c	and is_bash_reserved gives %d\n", b, j);
	
// 	b = '\'';
// 	j = is_bash_reserved(b);
// 	printf("b = %c	and is_bash_reserved gives %d\n", b, j);
	
// 	b = '\"';
// 	j = is_bash_reserved(b);
// 	printf("b = %c	and is_bash_reserved gives %d\n", b, j);
	
// 	b = '|';
// 	j = is_bash_reserved(b);
// 	printf("b = %c	and is_bash_reserved gives %d\n", b, j);
	
// 	b = '\\';
// 	j = is_bash_reserved(b);
// 	printf("b = %c	and is_bash_reserved gives %d\n", b, j);
	
// 	b = '@';
// 	j = is_bash_reserved(b);
// 	printf("b = %c	and is_bash_reserved gives %d\n", b, j);

	
// }
