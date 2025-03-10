/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pamatya <pamatya@student.42heilbronn.de    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/21 17:59:37 by pamatya           #+#    #+#             */
/*   Updated: 2025/02/02 12:23:36 by pamatya          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	main(int ac, char **av, char **envp)
{
	t_shell	shl;

	if (ac != 1)
		arg_error(av);
	init_shell(&shl, ac, av, envp);
	start_shell(&shl);
	reset_cmd_vars(&shl, 1, 1);
	clearout(&shl);
	return (shl.exit_code_prev);
}
