/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pamatya <pamatya@student.42heilbronn.de    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/01 22:26:38 by pamatya           #+#    #+#             */
/*   Updated: 2024/09/04 04:10:27 by pamatya          ###   ########.fr       */
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
	char *input;
	t_shell shl;

	if (ac != 1)
		arg_error(av);
	init_shell(&shl, envp);
	while (1)
	{
		input = readline("Mini-Schale $ ");
		if (!input)
			break ;
		if (!(ft_strncmp(input, "exit", 4)))
		{
			free(input);
			break ;
		}
		add_history(input);
		free(input);
	}
	ft_print_lst(shl.env);
	write(1, "\n", 1);
	write(1, "\n", 1);
	ft_print_lst(shl.env_paths);
	write(1, "\n", 1);
	write(1, "\n", 1);
	
	ft_lst_free(&shl.env);
	ft_lst_free(&shl.env_bak);
	ft_lst_free(&shl.env_paths);
	rl_clear_history();
	
	// clearout(&shl);
	ft_printf("Finished\n");
	return (0);
}

void	arg_error(char **av)
{
	ft_fprintf(2, "Minishell: %s: No such file or directory\n", av[1]);
	exit(127);
}

void	clearout(t_shell *shl)
{
	ft_lst_free(&shl->env);
	ft_lst_free(&shl->env_bak);
	ft_lst_free(&shl->env_paths);
	rl_clear_history();
}
