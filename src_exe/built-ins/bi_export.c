/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   bi_export.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pamatya <pamatya@student.42heilbronn.de    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/26 14:44:28 by pamatya           #+#    #+#             */
/*   Updated: 2025/01/01 21:45:46 by pamatya          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/minishell.h"

int			mini_export(t_shell *shl, t_cmds *cmd);
void		add_to_environ(t_shell *shl, t_cmds *cmd);
static int	check_export_syntax(char **args);

/*
Function for built-in export function
  - Adds the exported variable to the shl->env
  - Also adds the variable as key-val pair to the variables list


!!! This fn doesn't yet store env variables in the shl->environ (char **)

!!! Consider adding functionality for aliases with a simple file or just by
	adding a list
*/
int	mini_export(t_shell *shl, t_cmds *cmd)
{
	char	*_val;
	size_t	dp_len;

	dp_len = count_pointers(cmd->args);
	if (check_export_syntax(cmd->args) == -1)
		
	add_to_environ(shl, cmd);
	store_variable(shl, *(cmd->args + 1));
	
	update_env_var(shl, cmd, "_", NULL);
	return (0);
}

/*
Origin file: bi_export.c
*/
void	add_to_environ(t_shell *shl, t_cmds *cmd)
{
	size_t		dp_len;

	dp_len = count_pointers(shl->environ);
	shl->environ = ft_recalloc(shl->environ, (dp_len + 2) * 
			sizeof(*shl->environ), (dp_len + 1) * sizeof(*shl->environ));
	shl->environ[dp_len] = ft_strdup(*(cmd->args + 1));
	if (!shl->environ[dp_len])
		exit_early(shl, NULL, ERRMSG_MALLOC);
	shl->environ[dp_len + 1] = NULL;
}

static int	check_export_syntax(char **args)
{
	int	index;
	int	eq_count;
	int	i;

	index = 0;
	eq_count = 0;
	i = 0;
	while (args[++index])
	{
		if (args[index][i] == '=')
			return (-1);
		if (!ft_isalpha(args[index][i]) && args[index][i] != '_')
			return (-1);

		
		if ((ft_isalpha(args[index][i]) && ft_isalnum(args[index][i + 1]) == 0 && args[index][i + 1] != '_' && args[index][i + 1] != '=') || ())
			return (-1);
		
		
		while (args[index][++i])
		{
			if (args[index][i - 1] == '_' && args[index][i] == '=')
				return (-2);
			if (!ft_isalnum(args[index][i]) && args[index][i] != '_')
				return (-3);
		}
	}
}
