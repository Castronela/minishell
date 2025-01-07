/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   bi_pwd.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pamatya <pamatya@student.42heilbronn.de    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/26 14:44:34 by pamatya           #+#    #+#             */
/*   Updated: 2025/01/06 17:00:12 by pamatya          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/minishell.h"

int	mini_pwd(t_shell *shl, t_cmds *cmd)
{
	size_t	len;

	update_env_var(shl, cmd, UNDERSCORE, NULL);
	if (!shl->cur_wd)
		return (-1);
	len = ft_strlen(shl->cur_wd);
	write(cmd->fd_out, shl->cur_wd, len);
	write(cmd->fd_out, "\n", 1);
	return (0);
}
