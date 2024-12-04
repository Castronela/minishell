/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   b-i_echo.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pamatya <pamatya@student.42heilbronn.de    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/26 14:40:33 by pamatya           #+#    #+#             */
/*   Updated: 2024/09/26 14:41:55 by pamatya          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/minishell.h"

/*
Built-in echo function
  - Need to check if this is complete/sufficient...
  - if the command contains "-n ", then it writes to fd_out without a newline
  - if command contains no -n option, then it writes to fd_out followed by a newline
*/
void	exec_echo(t_shell *shl)
{
	char	**str;

	str = shl->cmds->cmds_lst->args;
	if (ft_strncmp(*str, "-n ", 3) == 0)
	{
		// echo with -n option
		ft_putstr_fd(*str + 1, shl->cmds->cmds_lst->fd_out);
	}
	else
	{
		// echo with no options
		ft_putstr_fd(*str, shl->cmds->cmds_lst->fd_out);
		write(shl->cmds->cmds_lst->fd_out, "\n", 1);
	}
}