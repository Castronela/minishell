/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_tolower.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dstinghe <dstinghe@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/03/11 10:28:54 by dstinghe          #+#    #+#             */
/*   Updated: 2024/03/15 11:01:22 by dstinghe         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

int	ft_tolower(int c)
{
	if (c >= 65 && c <= 90)
		return (c += - 'A' + 'a');
	return (c);
}

// #include <stdio.h>
// #include <ctype.h>
// int main()
// {
//     char letter = 'A';
//     printf("%c\n%c", tolower(letter), ft_tolower(letter));
// }