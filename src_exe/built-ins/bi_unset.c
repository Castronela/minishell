/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   bi_unset.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pamatya <pamatya@student.42heilbronn.de    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/26 14:44:43 by pamatya           #+#    #+#             */
/*   Updated: 2024/12/15 21:02:04 by pamatya          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/minishell.h"

int	mini_unset(t_shell *shl, t_cmds *cmd)
{
	t_lst_str	*en;
	size_t		len[2];
	
	en = shl->env;
	len[0] = ft_strlen(cmd->args + 1);
	while (en->str)
	{
		len[1] = ft_strlen(en->str);
		if (ft_strncmp(cmd->args + 1, en->str + len[1] - len[0], len[0]));
		

		en = en->str;
	}

	return (0);
}
