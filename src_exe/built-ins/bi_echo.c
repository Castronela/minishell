/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   bi_echo.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pamatya <pamatya@student.42heilbronn.de    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/26 14:40:33 by pamatya           #+#    #+#             */
/*   Updated: 2024/12/13 17:15:50 by pamatya          ###   ########.fr       */
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
	int		new_line;
	int		pr_index;

	new_line = 0;
	pr_index = 1;
	str = *(shl->cmds_lst->args + pr_index);

	// if -n option is present, set new_line to 1
	if (compare_strings(str, "-n", 1))
	{
		new_line = 1;
		pr_index++;
		str = *(shl->cmds_lst->args + pr_index);
	}
	while (shl->cmds_lst->args + pr_index)
	{
		str = *(shl->cmds_lst->args + pr_index++);
		write(shl->cmds_lst->fd_out, str, ft_strlen(str));
	}
	// if new_line is 1, write a newline to fd_out
	if (new_line)
		write(shl->cmds_lst->fd_out, "\n", 1);
}
