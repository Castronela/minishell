/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utilities.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pamatya <pamatya@student.42heilbronn.de    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/13 15:39:20 by pamatya           #+#    #+#             */
/*   Updated: 2025/01/14 03:48:11 by pamatya          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/minishell.h"

void	arg_error(char **av);
void	ft_free2dint(int **memory);
void	exit_early(t_shell *shl, char **double_ptr, char *msg);
void	exit_early2(t_shell *shl, char **double_ptr, char *s_ptr, char *msg);
void	clearout(t_shell *shl);
void	ft_print_lst(t_lst_str *root);

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
Function to free a 2D int pointer
*/
void	ft_free2dint(int **memory)
{
	size_t	i;

	i = 0;
	while ((memory != NULL) && (memory[i] != NULL))
	{
		free(memory[i]);
		memory[i] = NULL;
		i++;
	}
	free(memory);
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

void	exit_early2(t_shell *shl, char **double_ptr, char *s_ptr, char *msg)
{
	if (double_ptr)
		ft_free2d(double_ptr);
	if (s_ptr)
		free(s_ptr);
	if (shl)
	{
		reset_cmd_vars(shl, 1);
		clearout(shl);
	}
	if (msg && *msg)
		perror(msg);
	else if (msg && !*msg)
		ft_fprintf_str(STDERR_FILENO, (const char *[]){strerror(errno), NULL});
	exit(errno);
}

/*
Function to free any allocated memory during the minishell session and return
  - When called, the fn frees all allocated t_shell field pointers, clears the 
	readline history, and return control to the calling function.
  - This function is meant to exit minishell without memory leaks from pointers
	accesible from the main stack.
  - The funciton will not free any memory allocated locally by the calling fn
	which is not accessible through the main stack.
  - The function also does not free any t_cmds struct nodes with the assumption
	that this will occur within the minishell session at the end of each command
	cycle.
*/
void	clearout(t_shell *shl)
{
	// if (shl->environ != NULL)
	// 	ft_free2d(shl->environ);
	if (shl->environ != NULL)
		ft_free2d_safe(&shl->environ);
	if (shl->variables != NULL)
		ft_lst_free(&shl->variables);
	// if (shl->env_paths != NULL)
	// 	ft_lst_free(&shl->env_paths);
	if (shl->local_vars != NULL)
		ft_lst_free(&shl->local_vars);
	if (shl->cur_wd != NULL)
		ft_free_safe((void **)(&(shl->cur_wd)));
	if (shl->prompt != NULL)
		ft_free_safe((void **)(&(shl->prompt)));
	// if (shl->prompt != NULL)
	// 	free(shl->prompt);
	rl_clear_history();
	if (shl->stdio[0] != STDIN_FILENO)
	{
		if (close(shl->stdio[0]) < 0)
			exit_early(NULL, NULL, ERRMSG_CLOSE);
		printf("Closed the copy of stdin\n");
	}
	if (shl->stdio[1] != STDOUT_FILENO)
	{
		if (close(shl->stdio[1]) < 0)
			exit_early(NULL, NULL, ERRMSG_CLOSE);	
		printf("Closed the copy of stdout\n");
	}
}

void	ft_print_lst(t_lst_str *root)
{
	while (root)
	{
		ft_printf("%s\n", root->key);
		root = root->next;
	}
}


// #include <stdio.h>

// int main(void)
// {
// 	char	*arg = "-n";

// 	if (compare_strings("-n", arg, 1))
// 		printf("Same\n");
// 	else
// 		printf("Not the same\n");
// 	return (0);
// }
