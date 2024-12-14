/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   b-i_echo.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pamatya <pamatya@student.42heilbronn.de    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/26 14:40:33 by pamatya           #+#    #+#             */
/*   Updated: 2024/12/10 19:31:28 by pamatya          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/minishell.h"

/*
Built-in echo function
  - Need to check if this is complete/sufficient...
  - if the command contains "-n ", then it writes to fd_out without a newline
  - if command contains no -n option, then it writes to fd_out followed by a newline
*/
void	exe_echo(t_shell *shl)
{
	char	*str;
	int		nl;
	int		start_at;

	nl = 0;
	start_at = 1;
	str = *(shl->cmds_lst->args + start_at);
	
	// if -n option is present, set nl to 1
	if (ft_strncmp(str, "-n\0", 3) == 0)
	{
		nl = 1;
		start_at++;
		str = *(shl->cmds_lst->args + start_at);
	}
	write(shl->cmds_lst->fd_out, str, ft_strlen(str));
	// if nl is 1, write a newline to fd_out
	if (nl == 1)
		write(1, "\n", 1);
}
