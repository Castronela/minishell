/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strmapi.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pamatya <pamatya@student.42heilbronn.de    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/03/18 15:57:53 by pamatya           #+#    #+#             */
/*   Updated: 2024/06/12 23:46:35 by pamatya          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/libft.h"

char	*ft_strmapi(char const *s, char (*f)(unsigned int, char))
{
	char			*trnsfrmd;
	unsigned int	i;
	unsigned int	len;

	if (!s || !f)
		return (NULL);
	len = ft_strlen(s);
	i = 0;
	trnsfrmd = (char *)malloc((len + 1) * sizeof(char));
	if (!trnsfrmd)
		return (NULL);
	while (s[i])
	{
		trnsfrmd[i] = f(i, s[i]);
		i++;
	}
	trnsfrmd[i] = '\0';
	return (trnsfrmd);
}
