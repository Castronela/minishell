/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   bi_unset.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pamatya <pamatya@student.42heilbronn.de    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/26 14:44:43 by pamatya           #+#    #+#             */
/*   Updated: 2024/12/16 15:38:28 by pamatya          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/minishell.h"

int	mini_unset(t_shell *shl, t_cmds *cmd)
{
	t_lst_str	*en;
	size_t		len[2];
	
	en = shl->env;
	// len[0] = ft_strlen(cmd->args + 1);
	while (en->key)
	{
		// len[1] = ft_strlen(en->key);
		// if (ft_strncmp(cmd->args + 1, en->key + len[1] - len[0], len[0]));
		if (compare_strings(en->))
			
		en = en->key;
	}

	return (0);
}
