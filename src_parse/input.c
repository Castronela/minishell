/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   input.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pamatya <pamatya@student.42heilbronn.de    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/08 19:17:16 by castronela        #+#    #+#             */
/*   Updated: 2024/12/18 16:14:16 by pamatya          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void reset_cmd_vars(t_shell *shell, int free_before);

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
