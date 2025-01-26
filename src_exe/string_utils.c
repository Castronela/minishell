/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   string_utils.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pamatya <pamatya@student.42heilbronn.de    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/26 16:04:22 by pamatya           #+#    #+#             */
/*   Updated: 2025/01/26 15:39:43 by pamatya          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/minishell.h"

int		compare_strings(const char *str, const char *field, int exact);
char	**find_string_addr(t_shell *shl, char *str, int	n, int add_eq);
int		find_dptr_index(t_shell *shl, char *str, int n);
int		count_pointers(char **dp);
size_t	var_offset(char *str, int skip_separator);

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
Function to return the pointer to a string in a double pointer that matches the 
provided string
  - 'n' is the number of characters to match while finding the pointer
  - if 'add_eq' is 1, then one more character after the nth char is compared 
  	with '='
  - returns the address that stores the string if a match is found
  - returns NULL if no match is found
*/
char	**find_string_addr(t_shell *shl, char *str, int	n, int add_eq)
{
	int	i;

	i = -1;
	while (shl->environ[++i])
	{
		if (add_eq == 0)
		{
			if (ft_strncmp(shl->environ[i], str, n) == 0)
				return (&(shl->environ[i]));
		}
		else if (add_eq == 1)
		{
			if (ft_strncmp(shl->environ[i], str, n) == 0 &&
					shl->environ[i][n] == '=')
				return (&(shl->environ[i]));
		}
	}
	return (NULL);
}

/*
Function to return the index of the string in shl->environ
  - 'str' is the string being searched
  - 'n' is the number of characters to match with 'str' while finding the 
  	pointer index
  - Returns an int corresponding to its index in shl->environ
  - Returns -1 if a match is not found for 'str' in the array of strings
*/
int	find_dptr_index(t_shell *shl, char *str, int n)
{
	int		i;
	
	i = -1;
	while (shl->environ[++i])
	{
		if (ft_strncmp(shl->environ[i], str, n) == 0)
			return (i);
	}
	return (-1);
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
Function to skip the variable name in a variable-string.
  - Returns the total number of chars offset to skip the var_name
  - If 'skip_separator' is 1, the function also skips the separator encountered.
	Meaning, it returns the offset directly to the var. value.
	Possible separators: '=' and "+="
  - If 'skip_separator' is 0, the function only returns the offset to the
	separator and not the var. value
*/
size_t	var_offset(char *str, int skip_separator)
{
	int	i;

	i = 0;
	if (!str)
		return (0);
	while (*str && *str != '+' && *str != '=')
	{
		i++;
		str++;
	}
	if (skip_separator == 1)
	{
		if (*str == '+')
		{
			i++;
			str++;
		}
		if (*str == '=')
			i++;
	}
	return (i);
}
