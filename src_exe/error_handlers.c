/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utilities.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pamatya <pamatya@student.42heilbronn.de    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/13 15:39:20 by pamatya           #+#    #+#             */
/*   Updated: 2025/01/16 16:31:34 by pamatya          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/minishell.h"

void	arg_error(char **av);
void	exit_early(t_shell *shl, char **double_ptr, char *msg);

// void	ft_free2dint(int **memory);
// void	exit_early2(t_shell *shl, char **double_ptr, char *s_ptr, char *msg);

/*
Function to check if there are more than necessary args from user; if there is
then print an error message and exit
*/
void	arg_error(char **av)
{
	// ft_fprintf(2, "Minishell: %s: No such file or directory\n", av[1]);
	ft_putstr_fd("Minishell: ", 2);
	ft_putstr_fd(av[1], 2);
	ft_putstr_fd(": Syntax error; too many arguments\n", 2);
	ft_putstr_fd("Usage: ./<path to minishell executable>\n", 2);
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
		reset_cmd_vars(shl, 1);
		clearout(shl);
	}
	if (msg && *msg)
		perror(msg);
	else if (msg && !*msg)
		strerror(errno);
	exit(errno);
}

// void	exit_early2(t_shell *shl, char **double_ptr, char *s_ptr, char *msg)
// {
// 	if (double_ptr)
// 		ft_free2d(double_ptr);
// 	if (s_ptr)
// 		free(s_ptr);
// 	if (shl)
// 	{
// 		reset_cmd_vars(shl, 1);
// 		clearout(shl);
// 	}
// 	if (msg && *msg)
// 		perror(msg);
// 	else if (msg && !*msg)
// 		ft_fprintf_str(STDERR_FILENO, (const char *[]){strerror(errno), NULL});
// 	exit(errno);
// }

// /*
// Function to free a 2D int pointer
// */
// void	ft_free2dint(int **memory)
// {
// 	size_t	i;

// 	i = 0;
// 	while ((memory != NULL) && (memory[i] != NULL))
// 	{
// 		free(memory[i]);
// 		memory[i] = NULL;
// 		i++;
// 	}
// 	free(memory);
// }
