#include "minishell.h"

/*
Checks if token has unclosed quotes
Returns false and prints error on failure
*/
bool	is_quoteclosed(t_lsttoken *node_current)
{
	char quote;
	int i;

	quote = 0;
	i = -1;
	while (node_current->token[++i])
	{
		if (quote == 0 && is_qt(node_current->token[i]))
			quote = node_current->token[i];
		else if (quote == node_current->token[i])
			quote = 0;
	}
	if (quote != 0)
	{
		printf("%s\n", ERR_STX_QT);
		return (false);
	}
	return (true);
}

/*
Checks if operator repeats consecutively more often than expected
Returns 1 and prints error on failure
*/
bool	is_op_invalid(const char *op)
{
	if (!ft_strncmp(op, "<<", 2) || !ft_strncmp(op, ">>", 2))
	{
		if (op[2] != 0)
		{
			printf("%s `%s\'\n", ERR_STX_OP, &op[2]);
			return (false);
		}
		return (true);
	}
	else if (!ft_strncmp(op, "|", 1))
	{
		if (op[1] != 0)
		{
			printf("%s `%s\'\n", ERR_STX_OP, &op[1]);
			return (false);
		}
		return (true);
	}
	return (true);
}


/*
Checks if operator or argument after operator is invalid
Returns 1 on failure
*/
int	check_op(t_lsttoken *node_current, t_lsttoken *node_prev)
{
	if (is_op(*node_current->token))
	{
		if (is_op_invalid(node_current->token) == false)
			return (1);
		else if (*node_current->token == '|' && node_prev == NULL)
		{
			printf("%s `|\'\n", ERR_STX_OP);
			return (1);
		}
		else if (*node_current->token != '|' && node_current->next 
		&& is_op(*node_current->next->token))
		{
			printf("%s `%s\'\n", ERR_STX_OP, node_current->next->token);
			return (1);
		}
		else if (node_current->next == NULL)
		{
			printf("%s `newline\'\n", ERR_STX_OP);
			return (1);
		}
	}
	return (0);
}

/*
Checks syntax of command line
Returns 1 on failure, should not exit program
Caution: run fn before heredoc check!
*/
int	check_syntax(t_shell_data *shell)
{
	t_lsttoken *node_current;
	t_lsttoken *node_prev;

	if (shell->hd_status == true)
		return (0);
	node_prev = NULL;
	node_current = *shell->tokenlst;
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
