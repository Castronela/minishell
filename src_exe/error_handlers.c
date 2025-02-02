/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   error_handlers.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pamatya <pamatya@student.42heilbronn.de    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/13 15:39:20 by pamatya           #+#    #+#             */
/*   Updated: 2025/02/02 14:11:10 by pamatya          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/minishell.h"

void	arg_error(char **av);
void	exit_early(t_shell *shl, char **double_ptr, char *msg);
void	exit_early2(t_shell *shl, char **double_ptr, char *s_ptr, char *msg);

/*
Function to print error message when there is more than one argument
*/
void	arg_error(char **av)
{
	ft_fprintf_str(STDERR_FILENO, (const char *[]){ERSHL, av[1],
		" : No such file or directory\n", NULL});
	exit(127);
}

/*
Function for an early exit if there is some internal failure eg. malloc errors
  - Calls clearout() function, which frees all allocated t_shell fields
  - Calls lst_cmds_freelst() fn, that frees all t_cmds nodes and their fields
  - Frees any double char pointer allocated locally by the calling function
  - Prints an error message with perror() fn including the string 'msg'
  - Exits with the default global error variable 'errno'
*/
void	exit_early(t_shell *shl, char **double_ptr, char *msg)
{
	if (double_ptr)
		ft_free2d(double_ptr);
	if (shl)
	{
		reset_cmd_vars(shl, 1, 1);
		clearout(shl);
	}
	if (msg && *msg)
		perror(msg);
	else if (msg && !*msg)
		ft_fprintf_str(STDERR_FILENO, (const char *[]){strerror(errno), "\n",
			NULL});
	exit(errno);
}

void	exit_early2(t_shell *shl, char **double_ptr, char *s_ptr, char *msg)
{
	if (double_ptr)
		ft_free2d(double_ptr);
	if (s_ptr)
		free(s_ptr);
	if (shl)
	{
		reset_cmd_vars(shl, 1, 1);
		clearout(shl);
	}
	if (msg && *msg)
		perror(msg);
	else if (msg && !*msg)
		ft_fprintf_str(STDERR_FILENO, (const char *[]){strerror(errno), "\n",
			NULL});
	exit(errno);
}
