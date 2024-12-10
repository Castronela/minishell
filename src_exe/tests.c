/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   tests.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pamatya <pamatya@student.42heilbronn.de    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/23 14:26:50 by pamatya           #+#    #+#             */
/*   Updated: 2024/12/10 16:28:37 by pamatya          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/minishell.h"

void	test_initiations(t_shell *shl, int ac, char **av, char **envp)
{
	if (ac != 1)
		arg_error(av);
	init_shell(&shl, envp);
	while (1)
	{
		shl.cmdline = readline(shl.prompt);
		if (!shl.cmdline)
			break ;
		if (!(ft_strncmp(shl.cmdline, "exit", 4)))
		{
			free(shl.cmdline);
			break ;
		}
		add_history(shl.cmdline);
		free(shl.cmdline);
	}
	
	ft_print_lst(shl.env);
	write(1, "\n", 1);
	write(1, "\n", 1);
	// ft_print_lst(shl.env_bak);
	// write(1, "\n", 1);
	// write(1, "\n", 1);
	
	ft_print_lst(shl.env_paths);
	write(1, "\n", 1);
	write(1, "\n", 1);
	
	ft_printf("cwd: %s\n", shl.cur_wd);
	write(1, "\n", 1);
	write(1, "\n", 1);
	
	ft_printf("shlvl: %d\n", shl.shlvl);
	write(1, "\n", 1);
	write(1, "\n", 1);
	
	// shl.cmds->cmds_lst->args = ft_lst_new("Hello World");
	// exe_echo(shl.cmds->cmds_lst);
	// ft_lst_free(&shl.cmds->cmds_lst->args);

	clearout(&shl);
	ft_printf("Finished\n");
}
