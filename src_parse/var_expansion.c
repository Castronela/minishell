/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   var_expansion.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: david <david@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/17 15:49:37 by dstinghe          #+#    #+#             */
/*   Updated: 2024/12/18 01:03:13 by david            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void var_expand_args(t_shell *shell, t_cmds *cmd_node);
void		var_expansion(t_shell *shell, char **str);

static char	*expand_var(t_shell *shell, char *str, size_t *index);
static char	*get_var_name(t_shell *shell, const char *str, size_t index);
static char	*get_var_value(t_shell *shell, char *var_name);

void var_expand_args(t_shell *shell, t_cmds *cmd_node)
{
    size_t index;

    index = -1;
    while (cmd_node->args[++index])
        var_expansion(shell, &cmd_node->args[index]);
}

void	var_expansion(t_shell *shell, char **str)
{
	size_t	index;
	char	open_qt;

	open_qt = 0;
	index = 0;
	while ((*str)[index])
	{
		if (open_qt == 0 && is_quote((*str)[index]))
			open_qt = (*str)[index];
		else if (open_qt == (*str)[index])
			open_qt = 0;
		if ((*str)[index] == '$' && open_qt != SQ)
		{
			if ((*str)[index + 1] && (ft_isalpha((*str)[index + 1]) 
				|| (*str)[index + 1] == '_'))
				*str = expand_var(shell, *str, &index);
		}
		index++;
	}
}

static char	*expand_var(t_shell *shell, char *str, size_t *index)
{
	char	*str_expanded;
	char	*name_value[2];
	size_t	str_len;
	size_t	var_val_len[2];

	name_value[0] = get_var_name(shell, str, *index);
	var_val_len[0] = ft_strlen2(name_value[0]);
	name_value[1] = get_var_value(shell, name_value[0]);
	var_val_len[1] = ft_strlen2(name_value[1]);
	str_len = ft_strlen2(str);
	str_expanded = ft_calloc(str_len - var_val_len[0] + var_val_len[1] + 1,
			sizeof(*str_expanded));
	if (!str_expanded)
	{
		free(name_value[0]);
		free(name_value[1]);
		exit_early(shell, NULL, ERRMSG_MALLOC);
	}
	ft_strlcpy2(str_expanded, str, (*index + 1));
	ft_strlcpy2(&str_expanded[*index], name_value[1], var_val_len[1] + 1);
	ft_strlcat(str_expanded, &str[(*index) + var_val_len[0]], str_len
		- var_val_len[0] + var_val_len[1] + 1);
	(*index) += var_val_len[1] - 1;
	free(str);
	return (str_expanded);
}

static char	*get_var_name(t_shell *shell, const char *str, size_t index)
{
	size_t	start_index;
	char	*var_name;

	start_index = index++;
	while (str[index])
	{
		if (!ft_isalnum(str[index]) && str[index] != '_')
			break ;
		index++;
	}
	var_name = ft_substr(str, start_index, index - start_index);
	if (!var_name)
		exit_early(shell, NULL, ERRMSG_MALLOC);
	return (var_name);
}

static char	*get_var_value(t_shell *shell, char *var_name)
{
	char		*var_value;
	t_lst_str	*var_node;

	if (!var_name)
		return (NULL);
	var_value = NULL;
	var_node = shell->variables;
	while (var_node)
	{
		if (!ft_strncmp(var_node->key, var_name + 1, ft_strlen(var_node->key)
				+ 1))
		{
			var_value = var_node->val;
			break ;
		}
		var_node = var_node->next;
	}
	free(var_name);
	return (var_value);
}
