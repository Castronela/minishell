#include "minishell.h"

char *get_next_token(const char *cmdline, ssize_t *i_cmdline)
{
    ssize_t i_start;
    char first_char;

    i_start = *i_cmdline;
    first_char = cmdline[*i_cmdline];
    while(cmdline[++(*i_cmdline)])
    {
        if (check_quote(cmdline[*i_cmdline], i_cmdline, first_char))
            break ;
        if (is_whitespace(cmdline[*i_cmdline]) && is_quote(first_char) == false)
            break ;
        if (check_operator(cmdline[*i_cmdline], first_char))
            break ;
    }
    return (extract_str(cmdline, i_start, *i_cmdline));
}

int     tokenizer(t_shell_data *shell)
{
    ssize_t i_cmdline;
    ssize_t cmdline_len;
    char *token;

    i_cmdline = 0;
    cmdline_len = 0;
    if (shell->cmdline)
        cmdline_len = ft_strlen(shell->cmdline);
    while(i_cmdline < cmdline_len)
    {
        if (is_whitespace(shell->cmdline[i_cmdline]))
            i_cmdline++;
        else
        {
            token = get_next_token(shell->cmdline, &i_cmdline);
            if (token == NULL)
                return (1);
            if (tokenlst_addtoken(shell->tokenlst, token))
            {
                free(token);
                return (1);
            }
        }
    }
    return (0);
}
