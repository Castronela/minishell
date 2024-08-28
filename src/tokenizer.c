#include "minishell.h"

/*
Returns allocated string of closest token starting from index i_cmdline
Caution: cmdline != NULL
*/
char	*get_next_token(t_shell_data *shell, ssize_t *i_cmdline)
{
	ssize_t	i_start;
	char	first_char;

	shell->open_quote = 0;
	i_start = *i_cmdline;
	first_char = shell->cmdline[*i_cmdline];
	while (shell->cmdline[*i_cmdline])
	{
		if (shell->open_quote == 0 && shell->hd_status == false)
		{
			if (is_op(shell->cmdline[*i_cmdline]) && !is_op(first_char))
				break ;
			if (is_op(first_char) && first_char != shell->cmdline[*i_cmdline])
				break ;
			if (is_ws(shell->cmdline[*i_cmdline]))
				break ;
			if (is_qt(shell->cmdline[*i_cmdline]))
				shell->open_quote = shell->cmdline[*i_cmdline];
		}
		else if (shell->open_quote == shell->cmdline[*i_cmdline])
			shell->open_quote = 0;
		(*i_cmdline)++;
	}
	return (ft_substr(shell->cmdline, i_start, (*i_cmdline - i_start)));
}

/*
Assigns tokens from cmdline to tokenlst nodes
Caution: cmdline != NULL
*/
int	tokenizer(t_shell_data *shell)
{
	ssize_t	i_cmdline;
	char	*token;

	i_cmdline = 0;
	while (shell->cmdline[i_cmdline])
	{
		while (shell->hd_status == false && is_ws(shell->cmdline[i_cmdline]))
			i_cmdline++;
		if (shell->cmdline[i_cmdline])
		{
			token = get_next_token(shell, &i_cmdline);
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
