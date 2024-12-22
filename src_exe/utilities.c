/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utilities.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pamatya <pamatya@student.42heilbronn.de    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/13 15:39:20 by pamatya           #+#    #+#             */
/*   Updated: 2024/12/22 19:21:41 by pamatya          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/minishell.h"

int		compare_strings(const char *str, const char *field, int exact);
void	store_variable(t_shell *shl, char *str);
void	arg_error(char **av);
void	exit_early(t_shell *shl, char **split, char *msg);
void	clearout(t_shell *shl);
void	ft_print_lst(t_lst_str *root);

/*
The function is meant to compare two strings, and return 1 if they are same;
"exact" argument takes either 1 or 0.
  - Searches for the string 'str' in the possibly larger string 'field'
  - Returns 1 if the strings are same
  - Returns 0 if the strings differ
  - If the "exact" option is set to 1 during the call, the function will
    make sure that the strings are exactly the same, nothing more nothing less,
	i.e. it will also check if both strings are null-terminated at that position.
  - If the "exact" is set to 0, then null-termination is not checked.
  - In the case that the "exact" is off, the function will only check if
    the 'str' string is present in the 'field' string. In this case, the function
	will still also return 1.
*/
int	compare_strings(const char *str, const char *field, int exact)
{
	unsigned char	*s[2];

	if (!str || !field)
		return (-1);
	s[0] = (unsigned char *)str;
	s[1] = (unsigned char *)field;
	
	while (*s[0])
	{
		if (*s[0]++ != *s[1]++)
			return (0);
	}
	if (exact)
		if (s[0] != s[1])
			return (0);
	return (1);
}

/*
Function to add a new varaible to the minishell memory but not to shl->env
  - The parameter 'str' is split against '=' and ported intoft_lst_new fn
  - One variable is added upon per fn call
  - Does not write the variable to shl->env, this is only possible using the
    mini_export function
  - If malloc fails at any time, exit_early function is used to exit safely
*/
void	store_variable(t_shell *shl, char *str)
{
	t_lst_str	*new_var;
	char		**split;

	split = ft_split(str, '=');
	if (!split)
		exit_early(shl, NULL, "Could not split new variable string");
	new_var =ft_lst_new(*split, *(split + 1));
	if (!new_var)
		exit_early(shl, split, "Could not malloc new variable list node");
	ft_lst_addback(&shl->variables, new_var);
	ft_free2d(split);
}

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
	clearout(shl);
	lst_cmds_freelst(shl);
	if (double_ptr)
		ft_free2d(double_ptr);
	if (msg != NULL)
		perror(msg);
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
	if (shl->env_str!= NULL)
		ft_free2d(shl->env_str);
	if (shl->env != NULL)
		ft_lst_free(&shl->env);
	if (shl->variables != NULL)
		ft_lst_free(&shl->variables);
	if (shl->env_paths != NULL)
		ft_lst_free(&shl->env_paths);
	if (shl->cur_wd != NULL)
		free(shl->cur_wd);
	if (shl->prompt != NULL)
		free(shl->prompt);
	rl_clear_history();
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
