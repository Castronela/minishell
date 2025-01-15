/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   strings_utils.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pamatya <pamatya@student.42heilbronn.de    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/26 16:04:22 by pamatya           #+#    #+#             */
/*   Updated: 2025/01/16 18:02:16 by pamatya          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/minishell.h"

int		update_environ(char **var_ptr_addr, char *var_name, char *new_val);
int		compare_strings(const char *str, const char *field, int exact);
char	**find_string_addr(t_shell *shl, char *str, int	n);
int		find_dptr_index(t_shell *shl, char *str, int n);
int		count_pointers(char **dp);
size_t	offset_to_env_value(char *str);
int		is_bash_reserved(char c);

// int		cmp_cstr(const char *ndl, const char *hstack, int exact, int cased);

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
*/
char	**find_string_addr(t_shell *shl, char *str, int	n)
{
	int	i;
	
	i = -1;
	while (shl->environ[++i])
	{
		if (ft_strncmp(shl->environ[i], str, n) == 0)
			return (&(shl->environ[i]));
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
Function to return the address of the double pointer that holds the pointer to 
the string being searched in the double pointer, that matches the string 'str'
  - 'n' is the number of characters to match while finding the pointer
*/
// int	find_dptr_index(t_shell *shl, char *str, int n);

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
Funciton to replace the last char * in the shl->environ with new last-arg
*/
int	update_environ(char **var_ptr_addr, char *var_name, char *new_val)
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

// Function to get the length of the variable name upto '=' or '\0', whichever
// occurs first, including the '=' character if it occurs within 'str'.
size_t	offset_to_env_value(char *str)
{
	int	i;

	i = 0;
	if (!str)
		return (0);
	while (*str && *str != '=')
	{
		i++;
		str++;
	}
	if (*str == '=')
		i++;
	return (i);
}

/*
Function to check whether the char parameter is a bash-reserved character
  - Checks the character c within the array of special characters defined in the
	header file as bash-reserved characters using ft_strchr()
  - Returns 0 if the character is not found within the array
  - Returns 1 if the character is found within the array
*/
int	is_bash_reserved(char c)
{
	if (ft_strchr((const char []){BT, BN, DL, AD, SC, PO, PC, SQ, DQ, BS, PP, 
			'\0'}, (unsigned int)c) == NULL)
		return (0);
	return (1);
}

/*
Function same as compare_strings() fn, with an additional option to match the
case while comparing the strings.
  - If toggle 'cased' is 1, then case is also matched
  - If toggle 'cased' is 0, then case is disregarded
  - Returns 1 if the strings are same
  - Returns 0 if the strings differ
*/
// int	cmp_cstr(const char *ndl, const char *hstack, int exact, int cased)
// {
// 	unsigned char	*s[5];

// 	if (!ndl || !hstack)
// 		return (-1);
// 	s[1] = (unsigned char *)hstack;
// 	s[2] = (unsigned char *)ndl;
// 	s[3] = malloc((ft_strlen(ndl) + 1) * sizeof(unsigned char));
// 	// if (!s[3])
		
// 	s[4] = s[3];
// 	if (cased == 0)
// 	{
// 		while (*s[2])
// 			*(s[3])++ = ft_tolower(*(s[2])++);
// 		*s[3] = '\0';
// 		s[0] = s[4];
// 	}
// 	else
// 		s[0] = s[2];
// 	while (*s[0])
// 		if (*s[0]++ != *s[1]++)
// 			return (free(s[4]), 0);
// 	if (exact)
// 		if (*s[0] != *s[1])
// 			return (free(s[4]), 0);
// 	return (free(s[4]), 1);
// }
