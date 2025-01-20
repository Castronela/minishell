/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   indexing.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pamatya <pamatya@student.42heilbronn.de    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/16 00:47:44 by pamatya           #+#    #+#             */
/*   Updated: 2025/01/20 14:12:18 by pamatya          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/minishell.h"

void		index_cmds(t_shell *shl);
int			is_command(t_cmds *cmd);
int			is_built_in(t_cmds *cmds);

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

	total = 0;
	ext = 0;
	cmd = shl->cmds_lst;
	while (cmd)
	{
		total++;
		if (is_command(cmd))
		{
			cmd->cmd_index = total;
			if (cmd->args && !is_built_in(cmd))
				cmd->exc_index = ++ext;
		}
		else
			cmd->lvar_assignment = 1;
		cmd = cmd->next;
	}
	shl->total_cmds = total;
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
*/
int	is_command(t_cmds *cmd)
{
	char	**args;
	int		eq;

	args = cmd->args;
	while (args && *args)
	{
		eq = 0;
		if ((*args)[0] != '\0')
		{
			if ((*args)[0] == '=')
				return (1);
			if (!ft_isalpha((*args)[0]) && (*args)[0] != '_')
				return (1);
			if (check_eq_alnum(*args, &eq))
				return (1);
			if (!eq)
				return (1);
		}		
		args++;
		(cmd->skip)++;
	}
	return (0);
}

// Helper function for is_command()
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

// Function to check whether the command is in the built-in list
int	is_built_in(t_cmds *cmds)
{
	char	*arg;
	
	if (!cmds->args)
		return (1);
	arg = *(cmds->args + cmds->skip);
	if (compare_strings(arg, "echo", 1) || compare_strings(arg, "cd", 1) ||
		compare_strings(arg, "pwd", 1) || compare_strings(arg, "export", 1) ||
		compare_strings(arg, "unset", 1) || compare_strings(arg, "env", 1) ||
		compare_strings(arg, "exit", 1))
		return (1);
	else
		return (0);
}
