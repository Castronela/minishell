/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   string_utils_2.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pamatya <pamatya@student.42heilbronn.de    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/25 14:48:40 by pamatya           #+#    #+#             */
/*   Updated: 2025/02/02 20:19:04 by pamatya          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/minishell.h"

char	*concat_strings(const char *str[]);

/*
Function to malloc a total string by combining all the strings provided
  - Returns a bigger malloc'd string
  - Final string should be provided as NULL
*/
char	*concat_strings(const char *str[])
{
	size_t	indices[3];
	size_t	tlen;
	char	*concat;

	indices[0] = -1;
	tlen = 0;
	while (str[++indices[0]])
	{
		indices[1] = -1;
		while (str[indices[0]][++(indices[1])])
			tlen++;
	}
	concat = malloc((tlen + 1) * sizeof(char));
	if (!concat)
		return (NULL);
	indices[0] = -1;
	indices[2] = 0;
	while (str[++indices[0]])
	{
		indices[1] = 0;
		while (str[indices[0]][indices[1]])
			concat[(indices[2])++] = str[indices[0]][(indices[1])++];
	}
	concat[indices[2]] = '\0';
	return (concat);
}
