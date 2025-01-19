/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   bi_echo.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pamatya <pamatya@student.42heilbronn.de    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/26 14:40:33 by pamatya           #+#    #+#             */
/*   Updated: 2025/01/19 21:35:53 by pamatya          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/minishell.h"

void		mini_echo(t_cmds *cmd);
static void set_echo_flag(const char *str, int *new_line, 
			int *arg_index, int skp);

/*
Built-in echo function
  - Need to check if this is complete/sufficient...
  - if the command contains "-n ", then it writes to fd_out without a newline
  - if command contains no -n option, then it writes to fd_out followed by a newline

!!! > 25 lines
!!! shl->exit_code = 0 at the end of each built-in functions
*/
void	mini_echo(t_cmds *cmd)
{
	char	*str;
	int		new_line;
	int		arg_index;
	int		skp;

	new_line = 1;
	arg_index = cmd->skip + 1;
	skp = 1;
	str = *(cmd->args + arg_index);
	while (str && *str == '-' && *(str + 1) == 'n')
	{
		if (*(str + 2) != '\0' && *(str + 2) != 'n')
			break;
		skp++;
		set_echo_flag(str, &new_line, &arg_index, skp);
		str = *(cmd->args + arg_index);
	}
	while (*(cmd->args + arg_index))
	{
		str = *(cmd->args + arg_index++);
		write(STDOUT_FILENO, str, ft_strlen(str));
		if (*(cmd->args + arg_index))
			write(STDOUT_FILENO, " ", 1);
	}
	if (new_line)
		write(STDOUT_FILENO, "\n", 1);
}

/*
Function to deal with -n flag anomalies
*/
static void set_echo_flag(const char *str, int *new_line, int *arg_index, int skp)
{
	int fl_index;

	fl_index = 0;
	while(str[++fl_index])
	{
		if (str[fl_index] == 'n')
		{
			*new_line = 0;
			*arg_index = skp;
		}
		else
		{
			*new_line = 1;
			// *arg_index = 1;
			break ;
		}
	}
}
