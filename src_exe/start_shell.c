/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   start_shell.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dstinghe <dstinghe@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/17 21:46:09 by pamatya           #+#    #+#             */
/*   Updated: 2024/12/19 15:52:53 by dstinghe         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/minishell.h"

void		start_shell(t_shell *shl);
// static void	print_env(t_shell *shl);
// static void	print_shlvl(t_shell *shl);
// static void	print_cwd(t_shell *shl);
// static void	print_env_paths(t_shell *shl);

void	start_shell(t_shell *shl)
{
	while (1)
	{
		shl->cmdline = readline(shl->prompt);
		if (!shl->cmdline)
			break ;
		if (!(ft_strncmp(shl->cmdline, "exit", 4)))
		{
			free(shl->cmdline);
			break ;
		}
		add_history(shl->cmdline);
		parser(shl);
        heredoc(shl);
		init_pipes(shl);
        test_print_cmdlst(shl, 30);
		reset_cmd_vars(shl, 1);
	}

	// print_env(shl);
	// print_env_paths(shl);
	// print_cwd(shl);
	// print_shlvl(shl);
	
}