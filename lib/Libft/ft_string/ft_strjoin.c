/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strjoin.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pamatya <pamatya@student.42heilbronn.de    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/03/15 19:26:54 by pamatya           #+#    #+#             */
/*   Updated: 2024/09/02 02:08:12 by pamatya          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/libft.h"

char	*ft_strjoin(char const *s1, char const *s2)
{
	char	*j_str;
	size_t	len;
	size_t	i;

	if (!s1 || !s2)
		return (NULL);
	len = ft_strlen(s1) + ft_strlen(s2);
	j_str = (char *)malloc((len + 1) * sizeof(char));
	if (!j_str)
		return (NULL);
	i = 0;
	while (i < len)
	{
		if (i < ft_strlen(s1))
			j_str[i] = s1[i];
		else
			j_str[i] = *s2++;
		i++;
	}
	j_str[i] = '\0';
	return (j_str);
}
