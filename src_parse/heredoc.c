/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   heredoc.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dstinghe <dstinghe@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/17 15:23:48 by dstinghe          #+#    #+#             */
/*   Updated: 2024/12/18 18:18:02 by dstinghe         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

# include "minishell.h"

void heredoc(t_shell *shell);

static void check_heredoc(t_shell *shell, t_cmds *cmd_node);
static void var_expand_heredoc_body(t_shell *shell, t_lst_str *heredoc_node, int flag_expand_vars);
static void append_heredoc_body(t_shell *shell, t_lst_str *heredoc_node, char *input);

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
    int flag_expand_vars;
    char *input;

    heredoc_node = cmd_node->heredocs_lst;
    while (heredoc_node)
    {
        flag_expand_vars = count_closed_quotes(heredoc_node->key);
        remove_closed_quotes(shell, &heredoc_node->key);
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
        var_expand_heredoc_body(shell, heredoc_node, flag_expand_vars);
        heredoc_node = heredoc_node->next;
    }
}

/*
Expands variables of heredoc body ONLY if 'flag_expand_vars' is 0
    - encloses entire heredoc body in double quotes
    - performs variable expansion
    - removes double quotes added earlier
*/
static void var_expand_heredoc_body(t_shell *shell, t_lst_str *heredoc_node, int flag_expand_vars)
{
    char *new_hd_body;
    size_t hd_body_len;
    
    if (flag_expand_vars)
        return ;
    hd_body_len = ft_strlen2(heredoc_node->val);
    new_hd_body = ft_calloc(hd_body_len + 3, sizeof(*new_hd_body));
    if (!new_hd_body)
        exit_early(shell, NULL, ERRMSG_MALLOC);
    new_hd_body[0] = '\"';
    new_hd_body[hd_body_len] = '\"';
    ft_strlcpy2(&new_hd_body[1], heredoc_node->val, hd_body_len);
    var_expansion(shell, &new_hd_body);
    hd_body_len = ft_strlen2(new_hd_body);
    new_hd_body = ft_memmove(new_hd_body, &new_hd_body[1], hd_body_len);
    new_hd_body = ft_realloc(new_hd_body, hd_body_len + 1);
    free(heredoc_node->val);
    heredoc_node->val = new_hd_body;
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

