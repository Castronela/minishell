/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   bi_export.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pamatya <pamatya@student.42heilbronn.de    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/26 14:44:28 by pamatya           #+#    #+#             */
/*   Updated: 2024/12/31 14:52:54 by pamatya          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/minishell.h"

int		mini_export(t_shell *shl, t_cmds *cmd);
void	add_str_to_double_ptr(t_shell *shl, t_cmds *cmd);

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
	add_to_environment(shl, cmd);
	store_variable(shl, *(cmd->args + 1));
	return (0);
}
