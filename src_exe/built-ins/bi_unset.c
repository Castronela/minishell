/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   bi_unset.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pamatya <pamatya@student.42heilbronn.de    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/26 14:44:43 by pamatya           #+#    #+#             */
/*   Updated: 2024/12/14 18:17:33 by pamatya          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/minishell.h"

int	mini_unset(t_shell *shl, t_cmds *cmd)
{
	
	
	while (shl->env->)


	
	// Also store this in env_bak according to new layout proposed below.
	
	// Feature to be added inthe future:
	// the env_bak can be used to store a copy of the env but as a key:value
	// pair using another field, for easy extraction during variable expansion.
	// Also, this list may also be used to store other external vairables other
	// than env variables which may be created by the user during a session.
	return (0);
}
