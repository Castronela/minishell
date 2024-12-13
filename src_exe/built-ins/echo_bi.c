/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   echo_bi.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pamatya <pamatya@student.42heilbronn.de    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/26 14:40:33 by pamatya           #+#    #+#             */
/*   Updated: 2024/12/13 15:31:44 by pamatya          ###   ########.fr       */
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
	if (comp_str_abs(str, "-n", 1))
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

/*
The function is meant to compare two strings, and return 1 if they are same; "abs_toggle" argument takes either 1 or 0.
  - Returns 1 if the strings are same
  - Returns 0 if the strings differ
  - If the "abs_toggle" option is set to 1 during the call, the function will also compare if both strings are null-terminated at that position.
  - If the "abs_toggle" is set to 0, then null-termination is not checked.
  - In the case that the "abs_toggle" is off, the function will only check the strings until the point where even just one of the strings is null-terminated. In this case, the function will still return 1.
*/
int	comp_str_abs(const char *str1, const char *str2, int abs_toggle)
{
	unsigned char	*s[2];

	if (!str1 || !str2)
		return (-1);
	s[0] = (unsigned char *)str1;
	s[1] = (unsigned char *)str2;
	
	while (*s[0] && *s[1])
	{
		if (*s[0]++ != *s[1]++)
			return (0);
	}
	if (abs_toggle)
		if (s[0] != s[1])
			return (0);
	return (1);
}

#include <stdio.h>

int main(void)
{
	char	*arg = "-n";

	if (comp_str_abs(arg, "-n", 1))
		printf("Same\n");
	else
		printf("Not the same\n");
	return (0);
}
