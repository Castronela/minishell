/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   strings.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pamatya <pamatya@student.42heilbronn.de    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/26 16:04:22 by pamatya           #+#    #+#             */
/*   Updated: 2024/12/30 12:50:27 by pamatya          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/minishell.h"

int		compare_strings(const char *str, const char *field, int exact);
char	**find_string_ptr(char **dp, char *str, int	n);
int		update_var_str(char **var_ptr_addr, char *var_name, char *new_val);
int		count_pointers(char **dp);

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
		if (*s[0] != *s[1])
			return (0);
	return (1);
}

/*
Funcito returns the pointer to a string in a double pointer that matches the 
provided string
  - 'n' is the number of characters to match while finding the pointer
*/
char	**find_string_ptr(char **dp, char *str, int	n)
{
	int	i;
	
	i = -1;
	while (dp[++i])
	{
		if (ft_strncmp(dp[i], str, n) == 0)
			return (&dp[i]);
	}
	return (NULL);
}

/*
Function to count the number of char pointers contained in a double char pointer
  - Returns total number of pointers in a double pointer
  - Does not count the final NULL pointer, so the called has to account for it
	according to their use case
*/
int	count_pointers(char **dp)
{
	int	i;

	if (!dp || !*dp)
		return (0);
	i = 0;
	while (dp[i])
		i++;
	return (i); 
}

/*
Funciton to replace the last char * in the shl->environ list with new last-arg
*/
int	update_var_str(char **var_ptr_addr, char *var_name, char *new_val)
{
	char	*new;
	char	*tmp;

	tmp = ft_strdup(new_val);
	if (!tmp)
		return (-1);
	new = ft_strjoin(var_name, tmp);
	free(tmp);
	if (!new)
		return (-1);
	free(*var_ptr_addr);
	*var_ptr_addr = new;
	return (0);
}
