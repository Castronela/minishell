/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   input.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dstinghe <dstinghe@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/08 19:17:16 by castronela        #+#    #+#             */
/*   Updated: 2024/12/13 15:23:39 by dstinghe         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void get_normal_input(t_shell *shell);

static void reset_input_vars(t_shell *shell);

/*
Function in testing phase
*/
void get_normal_input(t_shell *shell)
{
    while (true)
    {
        reset_input_vars(shell);
        shell->cmdline = readline(shell->prompt);
        if (!shell->cmdline)
        {
            // TODO call exit command
            test_free_cmds(shell);
            printf("clean exit\n");
            exit(0);
        }
        add_history(shell->cmdline);
        parser(shell);
        test_print_cmdlst(shell);
        lst_cmds_freelst(shell);
        free(shell->cmdline);
    }
}

static void reset_input_vars(t_shell *shell)
{
    shell->cmdline = NULL;
    shell->cmds_lst = NULL;
    shell->open_qt = 0;
}
