/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   input.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dstinghe <dstinghe@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/08 19:17:16 by castronela        #+#    #+#             */
/*   Updated: 2024/12/17 21:03:44 by dstinghe         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void get_normal_input(t_shell *shell);
void reset_cmd_vars(t_shell *shell, int free_before);

/*
Function in testing phase
Run 'reset_input_vars' with second arg 0, before calling this function
*/
void get_normal_input(t_shell *shell)
{
    while (true)
    {
        reset_cmd_vars(shell, 1);
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
        heredoc(shell);
        test_print_cmdlst(shell, 30);
    }
}

/*
Nullifies all cmd vars
    - if 'free_before' is > 0: frees all non-null cmd vars before nullifying
*/
void reset_cmd_vars(t_shell *shell, int free_before)
{
    if (free_before)
    {
        if (shell->cmdline)
            free(shell->cmdline);
        if (shell->cmds_lst)
            lst_cmds_freelst(shell);
    }
    shell->cmdline = NULL;
    shell->cmds_lst = NULL;
    shell->open_qt = 0;
}
