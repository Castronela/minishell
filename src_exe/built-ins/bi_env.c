/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   bi_env.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pamatya <pamatya@student.42heilbronn.de    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/26 14:44:14 by pamatya           #+#    #+#             */
/*   Updated: 2025/01/24 02:22:12 by pamatya          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/minishell.h"

void	mini_env(t_shell *shl, t_cmds *cmd)
{
	int	i;

	update_env_var(shl, cmd, UNDERSCORE, NULL);
	i = -1;
	while (shl->environ[++i])
		printf("%s\n", shl->environ[i]);
}
