/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pamatya <pamatya@student.42heilbronn.de    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/21 17:59:37 by pamatya           #+#    #+#             */
/*   Updated: 2024/12/21 17:59:40 by pamatya          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

# include "minishell.h"

int	main(int ac, char **av, char **envp)
{
    t_shell  shl;

	if (ac != 1)
		arg_error(av);
	init_shell(&shl, envp);
	start_shell(&shl);
	clearout(&shl);
	ft_printf("Finished\n");
}
