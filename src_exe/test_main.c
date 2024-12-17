/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   test_main.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pamatya <pamatya@student.42heilbronn.de    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/23 14:26:50 by pamatya           #+#    #+#             */
/*   Updated: 2024/12/17 18:13:14 by pamatya          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/minishell.h"

void		test_initiations(t_shell *shl, int ac, char **av, char **envp);
// static void	print_env(t_shell *shl);
// static void	print_shlvl(t_shell *shl);
// static void	print_cwd(t_shell *shl);
// static void	print_env_paths(t_shell *shl);

void	test_initiations(t_shell *shl, int ac, char **av, char **envp)
{
	if (ac != 1)
		arg_error(av);
	init_shell(shl, envp);
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
		free(shl->cmdline);
	}

	

	// print_env(shl);
	// print_env_paths(shl);
	// print_cwd(shl);
	// print_shlvl(shl);
	
	clearout(shl);
	ft_printf("Finished\n");
}

// static void	print_env(t_shell *shl)
// {
// 	ft_print_lst(shl->env);
// 	write(1, "\n", 1);
// 	write(1, "\n", 1);
// 	// ft_print_lst(shl->variables);
// 	// write(1, "\n", 1);
// 	// write(1, "\n", 1);
// }

// static void	print_env_paths(t_shell *shl)
// {
// 	ft_print_lst(shl->env_paths);
// 	write(1, "\n", 1);
// 	write(1, "\n", 1);
// }

// static void	print_cwd(t_shell *shl)
// {
// 	ft_printf("cwd: %s\n", shl->cur_wd);
// 	write(1, "\n", 1);
// 	write(1, "\n", 1);
// }

// static void	print_shlvl(t_shell *shl)
// {
// 	ft_printf("shlvl: %d\n", shl->shlvl);
// 	write(1, "\n", 1);
// 	write(1, "\n", 1);
// }
