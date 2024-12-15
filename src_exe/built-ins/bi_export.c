/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   bi_export.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pamatya <pamatya@student.42heilbronn.de    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/26 14:44:28 by pamatya           #+#    #+#             */
/*   Updated: 2024/12/14 18:11:51 by pamatya          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/minishell.h"

int		mini_export(t_shell *shl, t_cmds *cmd);
void	store_variable(t_shell *shl, char *str);

/*
Function for built-in export function
*/
int	mini_export(t_shell *shl, t_cmds *cmd)
{
	t_lst_str	*new_var;
	char		**split;

	new_var = ft_lst_new(*(cmd->args + 1));
	if (!new_var)
		exit_early(shl, NULL, "Could not malloc new variable");
	ft_lst_addback(&shl->env, new_var);
	store_variable(shl, *(cmd->args + 1));
	return (0);
}
