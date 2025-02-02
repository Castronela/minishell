/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   bi_pwd.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pamatya <pamatya@student.42heilbronn.de    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/26 14:44:34 by pamatya           #+#    #+#             */
/*   Updated: 2025/02/02 18:48:37 by pamatya          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/minishell.h"

int	mini_pwd(t_shell *shl, t_cmds *cmd)
{
	size_t	len;

	update_env_var(shl, cmd, UNDERSCORE, NULL);
	if (!shl->cwd)
		return (-1);
	len = ft_strlen(shl->cwd);
	write(STDOUT_FILENO, shl->cwd, len);
	write(STDOUT_FILENO, "\n", 1);
	return (0);
}
