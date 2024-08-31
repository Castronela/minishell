#include "minishell.h"

/*
Replaces variable, from token, with its value, from environ
Returns NULL on failure
*/
char	*repl_var(char *token, char *variable, int *index)
{
	char	*str[2];
	int		len[3];

	if (*variable == 0)
		return (token);
	len[0] = ft_strlen(token);
	len[1] = ft_strlen(variable);
	len[2] = 0;
	str[0] = getenv(variable);
	if (str[0] != NULL)
		len[2] = ft_strlen(str[0]);
	str[1] = malloc(len[0] - len[1] + len[2]);
	if (str[1] == NULL)
		perror("Error malloc");
	else
	{
		ft_strlcpy(str[1], token, (*index) + 1);
		if (str[0] != NULL)
			ft_strlcpy(&str[1][*index], str[0], len[2] + 1);
		ft_strlcpy(&str[1][(*index) + len[2]], &token[(*index) + len[1] + 1],
			len[0] - (*index) - len[1]);
		(*index) += len[2] - 1;
	}
	free(token);
	return (str[1]);
}

/*
Returns variable name from token
Returns NULL on failure
*/
char	*get_var(const char *token, int i)
{
	char	*var;
	int		start;
	int		len;

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
	{
		perror("Error malloc");
		return (NULL);
	}
	ft_strlcpy(var, &token[start], len + 1);
	return (var);
}

/*
Replaces variables, from node, with their values
Returns 1 on failure
*/
int	repl_node_var(t_lsttoken *node)
{
	char	*var;
	char	quote;
	int		i;

	quote = 0;
	i = -1;
	while (node->token[++i])
	{
		if (quote == 0 && is_qt(node->token[i]))
			quote = node->token[i];
		else if (quote == node->token[i])
			quote = 0;
		if (node->token[i] == '$' && (quote == 0 || quote == '\"'))
		{
			var = get_var(node->token, i);
			if (var == NULL)
				return (1);
			node->token = repl_var(node->token, var, &i);
			free(var);
			if (node->token == NULL)
				return (1);
		}
	}
	return (0);
}

/*
Replaces variables, from all tokens, with their values
Returns 1 on failure
*/
int	repl_all_var(t_shell_data *shell)
{
	t_lsttoken	*node_current;

	node_current = *shell->tokenlst;
	while (node_current)
	{
		if (repl_node_var(node_current))
			return (1);
		node_current = node_current->next;
	}
	return (0);
}
