/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   bi_echo.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pamatya <pamatya@student.42heilbronn.de    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/26 14:40:33 by pamatya           #+#    #+#             */
/*   Updated: 2024/12/24 16:58:26 by pamatya          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/minishell.h"

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
	int		wr_index;

	new_line = 0;
	wr_index = 1;
	str = *(cmd->args + wr_index);

	// if -n option is present, set new_line to 1
	if (compare_strings("-n", str, 1))
	{
		new_line = 1;
		wr_index++;
		// str = *(cmd->args + wr_index);
	}
	while (cmd->args + wr_index)
	{
		str = *(cmd->args + wr_index);
		write(cmd->fd_out, str, ft_strlen(str));
		wr_index++;
	}
	// if new_line is 1, write a newline to fd_out
	if (new_line)
		write(cmd->fd_out, "\n", 1);
}




// #include <unistd.h>
// #include <string.h>
// #include <stdlib.h>

// int	compare_strings(const char *str, const char *field, int exact)
// {
// 	unsigned char	*s[2];

// 	if (!str || !field)
// 		return (-1);
// 	s[0] = (unsigned char *)str;
// 	s[1] = (unsigned char *)field;
	
// 	while (*s[0])
// 	{
// 		if (*s[0]++ != *s[1]++)
// 			return (0);
// 	}
// 	if (exact)
// 		if (*s[0] != *s[1])
// 			return (0);
// 	return (1);
// }

// void	mini_echo(char **args)
// {
// 	char	*str;
// 	int		new_line;
// 	int		wr_index;

// 	new_line = 0;
// 	wr_index = 1;
// 	str = *(args + wr_index);

// 	// if -n option is present, set new_line to 1
// 	if (compare_strings("-n", str, 1))
// 	{
// 		new_line = 1;
// 		wr_index++;
// 		str = *(args + wr_index);
// 	}
// 	while (args + wr_index)
// 	{
// 		str = *(args + wr_index++);
// 		write(1, str, strlen(str));
// 	}
// 	// if new_line is 1, write a newline to fd_out
// 	if (new_line)
// 		write(1, "\n", 1);
// }

// int main(void)
// {
// 	char	*s0 = strdup("echo");
// 	char	*s1 = strdup("Hello there");
// 	char	*s2 = strdup("world");
// 	char	**args;
// 	int		i;

// 	args = malloc(4 * sizeof(char *));
// 	args[0] = s0;
// 	args[1] = s1;
// 	args[2] = s2;
// 	args[3] = NULL;

// 	mini_echo(args);
// 	i = -1;
// 	while (++i)
// 		free(args[i]);
// }
