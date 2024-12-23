/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   start_shell.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dstinghe <dstinghe@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/17 21:46:09 by pamatya           #+#    #+#             */
/*   Updated: 2024/12/23 20:05:08 by dstinghe         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/minishell.h"

void		start_shell(t_shell *shl);
static void set_prev_exitcode(t_shell *shell);
// static void	print_env(t_shell *shl);
// static void	print_shlvl(t_shell *shl);
// static void	print_cwd(t_shell *shl);
// static void	print_env_paths(t_shell *shl);

void	start_shell(t_shell *shl)
{
	while (1)
	{
		set_signal(shl);
		set_prev_exitcode(shl);
		shl->cmdline = readline(shl->prompt);
		if (!shl->cmdline)
			break ;
		if (!(ft_strncmp(shl->cmdline, "exit", 4)))
			break ;
		add_history(shl->cmdline);
		if (parser(shl) || heredoc(shl))
		{
			reset_cmd_vars(shl, 1);
			continue ;
		}
		init_pipes(shl);
        test_print_cmdlst(shl, 30);
		reset_cmd_vars(shl, 1);
	}

	// print_env(shl);
	// print_env_paths(shl);
	// print_cwd(shl);
	// print_shlvl(shl);
	
}

static void set_prev_exitcode(t_shell *shell)
{
	shell->exit_code_prev = shell->exit_code;
	shell->exit_code = 0;
}
