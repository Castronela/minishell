/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   bi_exit.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pamatya <pamatya@student.42heilbronn.de    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/26 14:44:23 by pamatya           #+#    #+#             */
/*   Updated: 2024/12/29 19:16:47 by pamatya          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/minishell.h"

/*
Function to mimic exit command

!!! lst_cmds_freelst() fn call is causing seg-fault at the moment
*/
void	mini_exit(t_shell *shl)
{
	write(1, "exit\n", 5);
	lst_cmds_freelst(shl);			
	reset_cmd_vars(shl, 1);
	clearout(shl);
	exit(EXIT_SUCCESS);
}
