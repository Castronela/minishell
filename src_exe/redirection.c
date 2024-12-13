/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   redirection.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pamatya <pamatya@student.42heilbronn.de    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/13 13:04:04 by pamatya           #+#    #+#             */
/*   Updated: 2024/12/13 13:04:10 by pamatya          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "include/minishell.h"

int	redirect(int fd_old, int fd_new)
{
	if ((dup2(fd_new, fd_old)) == -1)
		return (-1);
	return (0);
}
