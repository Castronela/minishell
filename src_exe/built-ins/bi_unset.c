/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   bi_unset.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pamatya <pamatya@student.42heilbronn.de    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/26 14:44:43 by pamatya           #+#    #+#             */
/*   Updated: 2024/12/17 21:45:13 by pamatya          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/minishell.h"

/*
Function that mimics the built-in unset command in shell


!!! Needs to be checked for cases when the return code is not zero and also
	figure out how to handle returning that.
*/
int	mini_unset(t_shell *shl, t_cmds *cmd)
{
	t_lst_str	*del_node;
	char		*var_str;

	var_str = *(cmd->args + 1);
	del_node = ft_find_node(shl->env, var_str, 0, 0);
	ft_remove_node(&shl->env, del_node);
	del_node = ft_find_node(shl->variables, var_str, 1, 1);
	ft_remove_node(&shl->variables, del_node);
	return (0);
}
