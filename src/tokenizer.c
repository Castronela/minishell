#include "minishell.h"

/*
Returns allocated string of closest token starting from index i_cmdline
Caution: cmdline != NULL
*/
char	*get_next_token(t_shell *shell, ssize_t *i_cmdline)
{
	ssize_t	i_start;
	char	open_quote;

	open_quote = 0;
	i_start = *i_cmdline;
	while (shell->cmdline[*i_cmdline])
	{
		if (open_quote == 0)
		{
			if (op_in_token(shell->cmdline, i_start, i_cmdline))
				break ;
			if (is_ws(shell->cmdline[*i_cmdline]))
				break ;
			if (is_qt(shell->cmdline[*i_cmdline]))
				open_quote = shell->cmdline[*i_cmdline];
		}
		else if (open_quote == shell->cmdline[*i_cmdline])
			open_quote = 0;
		(*i_cmdline)++;
	}
	return (ft_substr(shell->cmdline, i_start, (*i_cmdline - i_start)));
}

/*
(Main FN) Populates tokenlst: adds new nodes with tokens from cmdline
On failure: returns 1 (to free: tokenlst)
Caution: cmdline != NULL
*/
int	tokenizer(t_shell *shell)
{
	ssize_t	i_cmdline;
	char	*token;

	i_cmdline = 0;
	while (shell->cmdline[i_cmdline])
	{
		while (is_ws(shell->cmdline[i_cmdline]))
			i_cmdline++;
		if (shell->cmdline[i_cmdline])
		{
			token = get_next_token(shell, &i_cmdline);
			if (token == NULL)
				return (1);
			if (add_token_to_lst(&shell->tokenlst, token))
				return (1);
		}
	}
	return (0);
}

/*
Adds new node with with value 'token' at the end of list 'root'
On failure: frees 'token' and returns 1
*/
int	add_token_to_lst(t_lst_str **root, char *token)
{
	t_lst_str *new;

	new = ft_lst_new(token);
	if (!new)
	{
		perror(ERR_MALLOC);
		free(token);
		token = NULL;
		return (1);
	}
	ft_lst_addback(root, new);
	return (0);
}

/*
Return true if str starts with a valid operator
OR if str at index i_cmdline has a valid operator
*/
bool	op_in_token(const char *str, int start, ssize_t *i_cmdline)
{
	ssize_t op_size;

	op_size = 0;
	if (is_op_redir_char(&str[start], &op_size) || is_op_ctrl_char(&str[start], &op_size)) 
	{
		*i_cmdline += op_size;
		return(true);
	}
	if ((is_op_redir_char(&str[*i_cmdline], &op_size) || is_op_ctrl_char(&str[*i_cmdline], &op_size))
	&& start != *i_cmdline)
		return(true);
	return(false);
}
