/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   tests.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pamatya <pamatya@student.42heilbronn.de    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/23 14:26:50 by pamatya           #+#    #+#             */
/*   Updated: 2025/01/16 16:25:59 by pamatya          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/minishell.h"

void	test_by_print(t_shell *shl)
{
	printf("no. of cmds:	%d\n", get_total_cmds(shl, 0));
	printf("no. of pids:	%d\n", get_total_cmds(shl, 1));
	// print_env(shl);
	// print_env_paths(shl);
	// print_cwd(shl);
	// print_shlvl(shl);	
	
}
	
void	ft_print_lst(t_lst_str *root)
{
	while (root)
	{
		ft_printf("%s:	%s\n", root->key, root->val);
		root = root->next;
	}
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
