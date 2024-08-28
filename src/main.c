/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pamatya <pamatya@student.42heilbronn.de    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/28 02:45:11 by pamatya           #+#    #+#             */
/*   Updated: 2024/08/28 02:46:57 by pamatya          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

# include "minishell.h"

int main(int ac, char **av)
{
    char        *input;
    // t_inputs history;
    
    (void) av;
    (void) ac;
    ft_putstr_fd("Welcome to Minishell!\n", 1);
    
    while (!(ft_strncmp(input, "exit", 4)))
    {
        input = readline("Mini-Schale$");
        if (!input)
            continue;
        add_history(input); 
    }
    
    return 0;
}
