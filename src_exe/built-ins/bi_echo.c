/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   bi_echo.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: david <david@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/26 14:40:33 by pamatya           #+#    #+#             */
/*   Updated: 2025/01/22 02:51:55 by david            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/minishell.h"

void		mini_echo(t_cmds *cmd);
static int	is_flag(const char *arg);

/*
Built-in echo function
  - Need to check if this is complete/sufficient...
  - if the command contains "-n ", then it writes to fd_out without a newline
  - if command contains no -n option,
	then it writes to fd_out followed by a newline

!!! > 25 lines
!!! shl->exit_code = 0 at the end of each built-in functions

!!! !!!Potentially needs to be re-written a bit for echo to print everything at
	once in a single buffer, including the optional newline.
*/
void	mini_echo(t_cmds *cmd)
{
	size_t	index;
	int		newline;
	char	**args;

	newline = 1;
	index = -1;
	args = cmd->args + cmd->skip + 1;
	while (args[++index] && is_flag(args[index]))
		newline = 0;
	while (args[index])
	{
		write(STDOUT_FILENO, args[index], ft_strlen(args[index]));
		if (args[index + 1])
			write(STDOUT_FILENO, " ", 1);
		index++;
	}
	if (newline)
		write(STDOUT_FILENO, "\n", 1);
}

static int	is_flag(const char *arg)
{
	size_t	index;

	index = 0;
	if (arg[index] != '-' || arg[index + 1] != 'n')
		return (0);
	else
		index += 2;
	while (arg[index] && arg[index] == 'n')
		index++;
	if (arg[index] == 0)
		return (1);
	return (0);
}
