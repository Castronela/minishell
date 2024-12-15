/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   strings.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pamatya <pamatya@student.42heilbronn.de    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/13 15:39:20 by pamatya           #+#    #+#             */
/*   Updated: 2024/12/13 17:14:02 by pamatya          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <include/minishell.h>

/*
The function is meant to compare two strings, and return 1 if they are same; "abs_toggle" argument takes either 1 or 0.
  - Returns 1 if the strings are same
  - Returns 0 if the strings differ
  - If the "abs_toggle" option is set to 1 during the call, the function will also compare if both strings are null-terminated at that position.
  - If the "abs_toggle" is set to 0, then null-termination is not checked.
  - In the case that the "abs_toggle" is off, the function will only check the strings until the point where even just one of the strings is null-terminated. In this case, the function will still return 1.
*/
int	compare_strings(const char *str1, const char *str2, int abs_toggle)
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

/*
Function to add a new varaible to the minishell memory but not to shl->env
  - The parameter 'str' is split against '=' and ported into ft_var_new fn
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
	new_var = ft_var_new(*split, *(split + 1));
	if (!new_var)
		exit_early(shl, split, "Could not malloc new variable list node");
	ft_lst_addback(&shl->variables, new_var);
	ft_free2d(split);
}

// #include <stdio.h>

// int main(void)
// {
// 	char	*arg = "-n";

// 	if (compare_strings(arg, "-n", 1))
// 		printf("Same\n");
// 	else
// 		printf("Not the same\n");
// 	return (0);
// }
