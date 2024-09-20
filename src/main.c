/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pamatya <pamatya@student.42heilbronn.de    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/01 22:26:38 by pamatya           #+#    #+#             */
/*   Updated: 2024/09/19 18:53:15 by pamatya          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/minishell.h"

// int		main(int ac, char **av);
int		main(int ac, char **av, char **envp);
void	arg_error(char **av);
void	clearout(t_shell *shl);

// int main(int ac, char **av)
int main(int ac, char **av, char **envp)
{
	t_shell shl;

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
	// exec_echo(shl.cmds->cmds_lst);
	// ft_lst_free(&shl.cmds->cmds_lst->args);

	clearout(&shl);
	ft_printf("Finished\n");
	return (0);
}

void	arg_error(char **av)
{
	// ft_fprintf(2, "Minishell: %s: No such file or directory\n", av[1]);
	ft_putstr_fd("Minishell: ", 2);
	ft_putstr_fd(av[1], 2);
	ft_putstr_fd(": No such file or directory\n", 2);
	exit(127);
}

void	clearout(t_shell *shl)
{
	ft_lst_free(&shl->env);
	ft_lst_free(&shl->env_bak);
	ft_lst_free(&shl->env_paths);
	free(shl->cur_wd);
	free(shl->prompt);
	// free(shl->prev_bin_path);
	rl_clear_history();
}
