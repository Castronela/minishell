/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   bi_pwd.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pamatya <pamatya@student.42heilbronn.de    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/26 14:44:34 by pamatya           #+#    #+#             */
/*   Updated: 2024/12/14 17:43:14 by pamatya          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/minishell.h"

int	mini_pwd(t_shell *shl, t_cmds *cmd)
{
	size_t	len;

	if (!shl->cur_wd)
		return (-1);
	len = ft_strlen(shl->cur_wd);
	write(cmd->fd_out, shl->cur_wd, len);
	return (0);
}
