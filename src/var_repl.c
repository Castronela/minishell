/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   var_repl.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: castronela <castronela@student.42.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/28 20:46:33 by castronela        #+#    #+#             */
/*   Updated: 2024/09/28 21:09:47 by castronela       ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static char	*subst_var_value(char *token, char *variable, ssize_t *var_index);
static char	*get_var(const char *token, ssize_t i);
static int	subst_vars(t_lst_str *node);

/*
(Main) Substitutes variables with their respective values
On failure: returns 1 (to free: tokenlst)
Caution: run before removing quotes!
*/
int	variable_expansion(t_shell *shell)
{
	t_lst_str	*node_current;

	node_current = shell->tokenlst;
	while (node_current)
	{
		if (subst_vars(node_current))
			return (1);
		node_current = node_current->next;
	}
	return (0);
}

/*
Substitutes variable, from token, with its value retrieved from environ
On failure: prints error, frees token, returns NULL
*/
static char	*subst_var_value(char *token, char *variable, ssize_t *var_index)
{
	char	*str[2];
	int		len[3];

	len[0] = ft_strlen(token);
	len[1] = ft_strlen(variable);
	str[0] = getenv(variable);
	len[2] = 0;
	if (str[0] != NULL)
		len[2] = ft_strlen(str[0]);
	str[1] = malloc(len[0] - len[1] + len[2]);
	if (str[1] == NULL)
		perror(ERR_MALLOC);
	else
	{
		ft_strlcpy(str[1], token, (*var_index) + 1);
		if (str[0] != NULL)
			ft_strlcpy(&str[1][*var_index], str[0], len[2] + 1);
		ft_strlcpy(&str[1][(*var_index) + len[2]], &token[(*var_index) + len[1]
			+ 1], len[0] - (*var_index) - len[1]);
		(*var_index) += len[2] - 1;
	}
	free(token);
	return (str[1]);
}

/*
Retrieves first variable from token
On failure: prints error, returns NULL
*/
static char	*get_var(const char *token, ssize_t i)
{
	char	*var;
	ssize_t	start;
	ssize_t	len;

	i++;
	start = i;
	len = 0;
	while (token[i] && ft_isalnum(token[i]))
	{
		i++;
		len++;
	}
	var = malloc(len + 1);
	if (var == NULL)
		perror(ERR_MALLOC);
	else
		ft_strlcpy(var, &token[start], len + 1);
	return (var);
}

/*
Substitutes all variables, from node, with their values
On failure: returns 1
*/
static int	subst_vars(t_lst_str *node)
{
	char	*var;
	char	quote;
	ssize_t	i;

	quote = 0;
	i = -1;
	while (node->str[++i])
	{
		if (quote == 0 && is_qt(node->str[i]))
			quote = node->str[i];
		else if (quote == node->str[i])
			quote = 0;
		if (node->str[i] == '$' && (quote == 0 || quote == '\"'))
		{
			var = get_var(node->str, i);
			if (var == NULL)
				return (1);
			node->str = subst_var_value(node->str, var, &i);
			free(var);
			if (node->str == NULL)
				return (1);
		}
	}
	return (0);
}
