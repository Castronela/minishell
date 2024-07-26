/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_toupper.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dstinghe <dstinghe@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/03/11 10:26:48 by dstinghe          #+#    #+#             */
/*   Updated: 2024/03/15 11:01:27 by dstinghe         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

int	ft_toupper(int c)
{
	if (c >= 97 && c <= 122)
		return (c += - 'a' + 'A');
	return (c);
}

// #include <stdio.h>
// #include <ctype.h>
// int main()
// {
//     char letter = 'T';
//     printf("%c\n%c", toupper(letter), ft_toupper(letter));
// }