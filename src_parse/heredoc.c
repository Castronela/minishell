/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   heredoc.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dstinghe <dstinghe@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/17 15:23:48 by dstinghe          #+#    #+#             */
/*   Updated: 2024/12/17 15:38:01 by dstinghe         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

# include "minishell.h"

void heredoc(t_shell *shell);

static void check_heredoc(t_shell *shell, t_cmds *cmd_node);
static void append_heredoc_body(t_shell *shell, t_lst_str *heredoc_node, char *input);

//TODO remove quotes from heredoc delimiter but do not expand it

/*
Loops through every command node and checks for open heredocs
*/
void heredoc(t_shell *shell)
{
    t_cmds *cmd_node;

    cmd_node = shell->cmds_lst;
    while (cmd_node)
    {
        check_heredoc(shell, cmd_node);
        cmd_node = cmd_node->next;
    }
}

/*
Loops through every heredoc node and processes heredoc body
    - on every heredoc node, prompts user for input
    - if input matches heredoc delimiter (key), breaks from loop
    and moves to next heredoc node or exits if null
    - if input doesn't match delimiter, calls function to append
    input to heredoc body and prompts user for input
*/
static void check_heredoc(t_shell *shell, t_cmds *cmd_node)
{
    t_lst_str *heredoc_node;
    char *input;

    heredoc_node = cmd_node->heredocs_lst;
    while (heredoc_node)
    {
        input = readline(PS2);
        while (input)
        {
            if (!ft_strncmp(heredoc_node->key, input, ft_strlen(heredoc_node->key) + 1))
            {
                free(input);
                break ;
            }
            else
                append_heredoc_body(shell, heredoc_node, input);
            input = readline(PS2);
        }
        heredoc_node = heredoc_node->next;
    }
}

/*
Appends 'input' and a newline to heredoc body
    - frees 'input' after appending
*/
static void append_heredoc_body(t_shell *shell, t_lst_str *heredoc_node, char *input)
{
    size_t heredoc_body_size;

    heredoc_body_size = ft_strlen(input) + 2;
    if (heredoc_node->val)
    {
        heredoc_body_size += ft_strlen(heredoc_node->val);
        heredoc_node->val = ft_realloc(heredoc_node->val, heredoc_body_size);
    }
    else
        heredoc_node->val = ft_calloc(heredoc_body_size, sizeof(*heredoc_node->val));
    if (!heredoc_node->val)
    {
        free(input);
        exit_early(shell, NULL, ERRMSG_MALLOC);
    }
    ft_strlcat(heredoc_node->val, input, heredoc_body_size);
    ft_strlcat(heredoc_node->val, "\n", heredoc_body_size);
    free(input);
}

