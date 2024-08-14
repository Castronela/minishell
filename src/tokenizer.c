#include "minishell.h"

/*
Returns allocated string of closest token starting from index i_cmdline
Caution: cmdline != NULL
*/
char	*get_next_token(const char *cmdline, ssize_t *i_cmdline)
{
	ssize_t	i_start;
	char	first_char;
	char	open_quote;

	open_quote = 0;
	i_start = *i_cmdline;
	first_char = cmdline[*i_cmdline];
	while (cmdline[*i_cmdline])
	{
		if (open_quote == 0)
		{
			if (is_operator(cmdline[*i_cmdline]) && !is_operator(first_char))
				break ;
			if (is_operator(first_char) && first_char != cmdline[*i_cmdline])
				break ;
			if (is_whitespace(cmdline[*i_cmdline]))
				break ;
		}
		if (is_quote(cmdline[*i_cmdline]) && open_quote == 0)
			open_quote = cmdline[*i_cmdline];
		else if (open_quote == cmdline[*i_cmdline])
			open_quote = 0;
		(*i_cmdline)++;
	}
	return (ft_substr(cmdline, i_start, (*i_cmdline - i_start)));
}

/*
Asigns tokens from cmdline to tokenlst nodes
Caution: cmdline != NULL
*/
int	tokenizer(t_shell_data *shell)
{
	ssize_t	i_cmdline;
	char	*token;

	i_cmdline = 0;
	while (shell->cmdline[i_cmdline])
	{
		while (is_whitespace(shell->cmdline[i_cmdline]))
			i_cmdline++;
		if (shell->cmdline[i_cmdline])
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
