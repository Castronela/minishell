/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expansions.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dstinghe <dstinghe@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/17 15:49:37 by dstinghe          #+#    #+#             */
/*   Updated: 2025/01/13 16:08:27 by dstinghe         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static char	*repl_var_w_value(t_shell *shell, char *str, size_t *index);
static char	*get_var_name(t_shell *shell, const char *str, size_t index);
static char	*get_var_value(t_shell *shell, char *var_name);

/*
Expands variables from 'str'
	- checks for '$' chars, not enclosed in single quotes
	- calls 'expand_var' if char following '$' is alphabetic
	or underscore
*/
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
		if ((*str)[index] == DOLLAR[0] && open_qt != SQ && (*str)[index + 1])
		{
			if ((*str)[index + 1] == '_' || ft_isalpha((*str)[index + 1])
				|| is_special_param(*str, index + 1))
				*str = repl_var_w_value(shell, *str, &index);
		}
		index++;
	}
}

/*
Substitues the first variable in string with the variable's value
	- retrieves the variable name and the variable's value
	- allocates memory for new string
	- copies first part of original string,
		up to the start of the variable name,
	to new string
	- concatenates variable's value to new string
	- concatenates rest of original string,
		starting from the end of variable name,
	to new string
	- frees original string and returns new string
Note: only call when 'str' at 'index' is a '$'
*/
static char	*repl_var_w_value(t_shell *shell, char *str, size_t *index)
{
	char	*str_expanded;
	char	*name_value[2];
	size_t	var_val_len[2];

	name_value[0] = get_var_name(shell, str, *index);
	var_val_len[0] = ft_strlen2(name_value[0]);
	name_value[1] = get_var_value(shell, name_value[0]);
	var_val_len[1] = ft_strlen2(name_value[1]);
	str_expanded = NULL;
	if (append_to_str(&str_expanded, str, *index)
		|| append_to_str(&str_expanded, name_value[1], -1)
		|| append_to_str(&str_expanded, &str[(*index) + var_val_len[0]], -1))
	{
		free(name_value[1]);
		exit_early(shell, NULL, ERRMSG_MALLOC);
	}
	free(name_value[1]);
	free(str);
	(*index) += var_val_len[1] - 1;
	return (str_expanded);
}

/*
Retrieves variable name, starting from $
	- reads and returns string after '$'
	- the string must contain either one single special character or
	an array of alphanumeric and underscore characters
*/
static char	*get_var_name(t_shell *shell, const char *str, size_t index)
{
	size_t	start_index;
	char	*var_name;

	start_index = index++;
	if (is_special_param(str, start_index + 1))
		index++;
	else
	{
		while (ft_isalnum(str[index]) || str[index] == '_')
			index++;
	}
	var_name = ft_substr(str, start_index, index - start_index);
	if (!var_name)
		exit_early(shell, NULL, ERRMSG_MALLOC);
	return (var_name);
}

/*
Finds and returns value of variable 'var_name'
*/
static char	*get_var_value(t_shell *shell, char *var_name)
{
	char		*var_value;
	t_lst_str	*var_node;

	if (!var_name)
		return (NULL);
	var_value = NULL;
	if (is_special_param(var_name, 1))
	{
		if (!ft_strncmp(var_name + 1, QUESTION_MARK, ft_strlen(QUESTION_MARK)
				+ 1))
			var_value = ft_itoa(shell->exit_code_prev);
	}
	else
	{
		var_node = ft_find_node(shell->variables, var_name + 1, 0, 1);
		if (!var_node)
			var_node = ft_find_node(shell->local_vars, var_name + 1, 0, 1);
		if (var_node)
			var_value = ft_strdup(var_node->val);
	}
	free(var_name);
	return (var_value);
}

void	expand_homedir_special_char(t_shell *shell, char **str)
{
	t_lst_str	*lst_node;
	size_t		index;
	char		*new_str;

	new_str = NULL;
	if (compare_strings("~", *str, 1) || compare_strings("~/", *str, 0))
		lst_node = ft_find_node(shell->variables, "HOME", 0, 1);
	else if (compare_strings("~+", *str, 1) || compare_strings("~+/", *str, 0))
		lst_node = ft_find_node(shell->variables, "PWD", 0, 1);
	else if (compare_strings("~-", *str, 1) || compare_strings("~-/", *str, 0))
		lst_node = ft_find_node(shell->variables, "OLDPWD", 0, 1);
	else
		return ;
	if (!lst_node || !lst_node->val)
		return ;
	index = 1;
	while ((*str)[index] && (*str)[index] != '/')
		index++;
	if (append_to_str(&new_str, lst_node->val, -1) || append_to_str(&new_str,
			&(*str)[index], -1))
		exit_early(shell, NULL, ERRMSG_MALLOC);
	free(*str);
	*str = new_str;
}
