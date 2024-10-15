/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   input_checker.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: david <david@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/28 20:46:21 by castronela        #+#    #+#             */
/*   Updated: 2024/10/15 19:59:03 by david            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static bool	is_quoteclosed(t_lst_str *node_current);
static int	check_op(t_lst_str *node_current);

/*
(Main FN) Checks syntax of command line
On failure: returns 1 (nothing to free)
Caution: run fn before heredoc check! should not exit program!
*/
int	check_syntax(t_shell *shell)
{
	t_lst_str	*node_current;

	if (shell->hd_status == true)
		return (0);
	node_current = shell->tokenlst;
	while (node_current)
	{
		if (check_op(node_current))
			return (1);
		if (!node_current->next && is_quoteclosed(node_current) == false)
			return (1);
		node_current = node_current->next;
	}
	return (0);
}

/*
Checks if token has unclosed quotes
On failure: prints error and returns false
*/
static bool	is_quoteclosed(t_lst_str *node_current)
{
	char	quote;
	int		i;

	quote = 0;
	i = -1;
	while (node_current->str[++i])
	{
		if (quote == 0 && multicmp(&node_current->str[i], (const char *[]){QT,
				NULL}, 0))
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
Checks validity of syntax when token is redirection or control operator.
When token is redirection: must be followed by non-NULL and non-operator token.
When token is control: must have non-NULL tokens before and after.
On failure: prints error and returns 1
*/
static int	check_op(t_lst_str *node_current)
{
	if (multicmp(node_current->str, (const char *[]){RD, HD, NULL}, 1))
	{
		if (!node_current->next)
		{
			printf("%s `newline\'\n", ERR_STX_UNEXP_TOKEN);
			return (1);
		}
		else if (multicmp(node_current->next->str, (const char *[]){RD, HD, CT,
				NULL}, 1))
		{
			printf("%s `%s\'\n", ERR_STX_UNEXP_TOKEN, node_current->next->str);
			return (1);
		}
	}
	else if (multicmp(node_current->str, (const char *[]){CT, NULL}, 1)
		&& (!node_current->prev || !node_current->next))
	{
		printf("%s\n", ERR_STX_INCOMPLETE_OP);
		return (1);
	}
	return (0);
}
