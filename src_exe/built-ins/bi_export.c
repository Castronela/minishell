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

int	mini_export(t_shell *shl, t_cmds *cmd)
{
	t_lst_str	*new_var;

	new_var = ft_lst_new(*(cmd->args) + 1);
	if (!new_var)
		return (-1);
	ft_lst_addback(&shl->env, new_var);
	// Also store this in env_bak according to new layout proposed below.
	
	// Feature to be added inthe future:
	// the env_bak can be used to store a copy of the env but as a key:value
	// pair using another field, for easy extraction during variable expansion.
	// Also, this list may also be used to store other external vairables other
	// than env variables which may be created by the user during a session.
	return (0);
}
