/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   bi_exit.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: david <david@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/26 14:44:23 by pamatya           #+#    #+#             */
/*   Updated: 2025/01/15 00:22:48 by david            ###   ########.fr       */
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
	reset_cmd_vars(shl, 1);
	clearout(shl);
	exit(EXIT_SUCCESS);
}
