#include "minishell.h"

/*
Checks if token has unclosed quotes
On failure: prints error and returns false
*/
bool	is_quoteclosed(t_lst_str *node_current)
{
	char	quote;
	int		i;

	quote = 0;
	i = -1;
	while (node_current->str[++i])
	{
		if (quote == 0 && is_qt(node_current->str[i]))
			quote = node_current->str[i];
		else if (quote == node_current->str[i])
			quote = 0;
	}
	if (quote != 0)
	{
		printf("%s\n", ERR_STX_OPEN_QT);
		return (false);
	}
	return (true);
}

/*
Checks if operator repeats consecutively more often than expected
On failure: prints error and returns 1
*/
int	check_op(t_lst_str *node_current, t_lst_str *node_prev)
{
	if (is_op_redir_char(node_current->str, NULL))
	{
		if (!node_current->next)
		{
			printf("%s `newline\'\n", ERR_STX_UNEXP_TOKEN);
			return (1);
		}
		else if (is_op_redir_char(node_current->next->str, NULL) 
			|| is_op_ctrl_char(node_current->next->str, NULL))
		{
			printf("%s `%s\'\n", ERR_STX_UNEXP_TOKEN, node_current->next->str);
			return (1);
		}
	}
	if (is_op_ctrl_char(node_current->str, NULL) && (!node_prev || !node_current->next))
	{
		printf("%s\n", ERR_STX_INCOMPLETE_OP);
		return (1);
	}
	return (0);
}

/*
(Main FN) Checks syntax of command line
On failure: returns 1 (nothing to free)
Caution: run fn before heredoc check! should not exit program!
*/
int	check_syntax(t_shell *shell)
{
	t_lst_str	*node_current;
	t_lst_str	*node_prev;

	if (shell->hd_status == true)
		return (0);
	node_prev = NULL;
	node_current = shell->tokenlst;
	while (node_current)
	{
		if (check_op(node_current, node_prev))
			return (1);
		if (is_quoteclosed(node_current) == false)
			return (1);
		node_prev = node_current;
		node_current = node_current->next;
	}
	return (0);
}
