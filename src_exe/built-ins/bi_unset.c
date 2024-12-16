/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   bi_unset.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pamatya <pamatya@student.42heilbronn.de    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/26 14:44:43 by pamatya           #+#    #+#             */
/*   Updated: 2024/12/16 20:53:01 by pamatya          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/minishell.h"

int	mini_unset(t_shell *shl, t_cmds *cmd)
{
	t_lst_str	*en;
	
	en = shl->env;
	while (en->val)
	{
		if (compare_strings(*(cmd->args + 1), en->val, 1))
			
		en = en->key;
	}

	return (0);
}
