/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   bi_echo.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: david <david@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/26 14:40:33 by pamatya           #+#    #+#             */
/*   Updated: 2024/12/24 21:06:55 by david            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/minishell.h"

static void set_echo_flag(const char *str, int *new_line, int *pr_index);

/*
Built-in echo function
  - Need to check if this is complete/sufficient...
  - if the command contains "-n ", then it writes to fd_out without a newline
  - if command contains no -n option, then it writes to fd_out followed by a newline
*/
void	mini_echo(t_cmds *cmd)
{
	char	*str;
	int		new_line;
	int		pr_index;

	new_line = 1;
	pr_index = 1;
	str = *(cmd->args + pr_index);

	// if -n option is present, set new_line to 0
	if (*str == '-')
		set_echo_flag(str, &new_line, &pr_index);
	while (*(cmd->args + pr_index))
	{
		str = *(cmd->args + pr_index++);
		write(cmd->fd_out, str, ft_strlen(str));
		if (*(cmd->args + pr_index))
			write(cmd->fd_out, " ", 1);
	}
	// if new_line is 1, write a newline to fd_out
	if (new_line)
		write(cmd->fd_out, "\n", 1);
}

static void set_echo_flag(const char *str, int *new_line, int *pr_index)
{
	int fl_index;

	fl_index = 0;
	while(str[++fl_index])
	{
		if (str[fl_index] == 'n')
		{
			*new_line = 0;
			*pr_index = 2;
		}
		else
		{
			*new_line = 1;
			*pr_index = 1;
			break ;
		}
	}
}
