/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   indexing.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pamatya <pamatya@student.42heilbronn.de    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/16 00:47:44 by pamatya           #+#    #+#             */
/*   Updated: 2025/01/16 18:59:11 by pamatya          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/minishell.h"

void		index_cmds(t_shell *shl);
int			is_command(t_cmds *cmd);
int			is_built_in(char *cmd);

static int	check_eq_alnum(char *arg, int *eq);

/*
Function that indexes all commands as well as all external commands from 1 to n
  - Returns the total number of commands, internal + external
*/
void	index_cmds(t_shell *shl)
{
	t_cmds	*cmd;
	int		total;
	int		ext;

	total = 1;
	ext = 1;
	cmd = shl->cmds_lst;
	while (cmd)
	{
		if (is_command(cmd))
		{
			cmd->cmd_index = total++;
			if (cmd->args && !is_built_in(*(cmd->args + cmd->skip)))
				cmd->exc_index = ext++;
		}
		else
			cmd->lvar_assignment = 1;
		cmd = cmd->next;
	}
}

/*
Function that checks if any of the arguments in the cmd struct is a command
  - Returns 1 when it finds the first command, i.e. either the variable name is 
	invalid or the '=' is not present in at least one of the args
  - Returns 0 if no commands are found and all args are proper variables, i.e.
	all args are valid variable names containing an '=' sign
  - The value at the address stored by the 'skip' pointer is incremented to
	count the total number of valid variable arguments found before a command
	was encountered
  - The value at 'skip' is for the purpose of indicating how many arguments to
	skip to get to the argument where the command and its arguments start

!!! Function too long
*/
int	is_command(t_cmds *cmd)
{
	char	**args;
	int		eq;

	args = cmd->args;
	while (args && *args)
	{
		eq = 0;
		if ((*args)[0] == '=')
			return (1);
		if (!ft_isalpha((*args)[0]) && (*args)[0] != '_')
			return (1);
		if (check_eq_alnum(*args, &eq))
			return (1);
		if (!eq)
			return (1);
		args++;
		(cmd->skip)++;
	}
	return (0);
}

static int	check_eq_alnum(char *arg, int *eq)
{
	int	i;
	
	i = 0;
	while (arg[++i])
	{
		if (arg[i] == '=')
		{
			*eq = 1;
			break ;
		}
		if (!ft_isalnum(arg[i]) && arg[i] != '_')
			return (1);
	}
	return (0);
}

// int	is_command_old(t_cmds *cmd)
// {
// 	char	**args;
// 	int		i;
// 	int		eq;

// 	args = cmd->args;
// 	while (*args)
// 	{
// 		eq = 0;
// 		if ((*args)[0] == '=')
// 			return (1);
// 		if (!ft_isalpha((*args)[0]) && (*args)[0] != '_')
// 			return (1);
// 		i = 0;
// 		while ((*args)[++i])
// 		{
// 			if ((*args)[i] == '=')
// 			{
// 				eq = 1;
// 				break ;
// 			}
// 			if (!ft_isalnum((*args)[i]) && (*args)[i] != '_')
// 				return (1);
// 		}
// 		if (!eq)
// 			return (1);
// 		args++;
// 		(cmd->skip)++;
// 	}
// 	return (0);
// }

// Function to check whether the command is in the built-in list
int	is_built_in(char *cmd)
{
	if (compare_strings(cmd, "echo", 1) || compare_strings(cmd, "cd", 1) ||
		compare_strings(cmd, "pwd", 1) || compare_strings(cmd, "export", 1) ||
		compare_strings(cmd, "unset", 1) || compare_strings(cmd, "env", 1) ||
		compare_strings(cmd, "exit", 1))
		return (1);
	else
		return (0);
}
