/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils_4.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pamatya <pamatya@student.42heilbronn.de    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/16 00:47:48 by pamatya           #+#    #+#             */
/*   Updated: 2025/02/02 20:29:06 by pamatya          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/minishell.h"

void	restore_std_fds(t_shell *shl);
int		path_is_dir(char *path);
int		is_valid_name(char *arg, int *i);
int		get_append_flag(int check);

/*
Function to restore the STDIN_FILENO and STDOUT_FILENO to point to the terminal
*/
void	restore_std_fds(t_shell *shl)
{
	if ((dup2(shl->stdio[0], STDIN_FILENO)) == -1)
		exit_early(shl, NULL, ERRMSG_DUP2);
	if ((dup2(shl->stdio[1], STDOUT_FILENO)) == -1)
		exit_early(shl, NULL, ERRMSG_DUP2);
}

/*
Function to check whether the given path is a valid path or not
  - Returns 1 if it is a valid path
  - Returns 0 if the path is invalid, or if the path results in a file
*/
int	path_is_dir(char *path)
{
	int			i;
	struct stat	bufr;

	i = stat(path, &bufr);
	if (i < 0)
		return (0);
	else if (i == 0)
	{
		if (S_ISREG(bufr.st_mode))
			return (0);
		else if (S_ISDIR(bufr.st_mode))
			return (1);
	}
	return (0);
}

/*
Function to get the append flag for type of append to be performed
*/
int	get_append_flag(int check)
{
	int	append;

	append = 0;
	if (check == 0)
		append = -1;
	else if (check == 1)
		append = 0;
	else if (check == 3)
		append = 1;
	return (append);
}

/*
Function to check whether variable name is valid
  - chex if the name starts with alphabet or '_', and that the characters
	following that are alphanumeric
  - Returns 1 if the name is valid
  - Returns 0 if no '=' sign is found in the argument
  - Returns -n if the nth char in the var_name string is found to be invalid
  - Returns 2 if the variable name is just "_"
  - Used fns: ft_isalpha(), ft_isalnum()
*/
int	is_valid_name(char *arg, int *i)
{
	int	*index;
	int	local_i;

	local_i = 0;
	if (i != NULL)
		index = i;
	else
		index = &local_i;
	if (arg[0] == '=')
		return (-1);
	if (!ft_isalpha(arg[0]) && arg[0] != '_')
		return (-1);
	if ((arg[0] == '_' && arg[1] == '=')
		|| (arg[0] == '_' && arg[1] == '+' && arg[2] == '='))
		return (2);
	while (arg[++(*index)])
	{
		if (arg[*index] == '=')
			return (++(*index), 1);
		if (arg[*index] == '+' && arg[*index + 1] == '=')
			return (*index = *index + 2, 3);
		if (!ft_isalnum(arg[*index]) && arg[*index] != '_')
			return (-1 * (*index));
	}
	return (0);
}
