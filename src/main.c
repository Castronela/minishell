/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pamatya <pamatya@student.42heilbronn.de    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/01 22:26:38 by pamatya           #+#    #+#             */
/*   Updated: 2024/09/03 01:49:21 by pamatya          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/minishell.h"

// int		main(int ac, char **av);
int		main(int ac, char **av, char **envp);
void	arg_error(char **av);

// int main(int ac, char **av)
int main(int ac, char **av, char **envp)
{
	char *input;
	t_shell shl;

	if (ac != 1)
		arg_error(av);
	init_shell(&shl, envp);
	ft_putstr_fd("Welcome to Minishell!\n", 1);
	using_history();
	while (1)
	{
		// input = readline("Mini-Schale$");
		input = readline("Mini-Schale $ ");
		if (!input)
			continue;
		else if (!(ft_strncmp(input, "exit", 4)))
		{
			free(input);
			break ;
		}
		add_history(input);
		free(input);
	}
	rl_clear_history();
	return (0);
}

void	arg_error(char **av)
{
	ft_fprintf(2, "Minishell: %s: No such file or directory\n", av[1]);
	exit(127);
}

void init_shell()
{
	
}