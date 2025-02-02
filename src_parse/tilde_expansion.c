/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   tilde_expansion.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dstinghe <dstinghe@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/29 01:00:14 by david             #+#    #+#             */
/*   Updated: 2025/02/02 21:56:10 by dstinghe         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void		expand_homedir_special_char(t_shell *shell, char **str);

static char	*get_home_path(t_shell *shell, const char *str);

void	expand_homedir_special_char(t_shell *shell, char **str)
{
	char	*home_path;
	char	*new_str;
	size_t	index;

	new_str = NULL;
	home_path = get_home_path(shell, *str);
	if (!home_path)
		return ;
	index = 1;
	while ((*str)[index] && (*str)[index] != '/')
		index++;
	if (append_to_str(&new_str, home_path, -1) || append_to_str(&new_str,
			&(*str)[index], -1))
		exit_early(shell, NULL, ERRMSG_MALLOC);
	free(*str);
	*str = new_str;
}

static char	*get_home_path(t_shell *shell, const char *str)
{
	t_lst_str	*lst_node;

	lst_node = NULL;
	if (compare_strings("~", str, 1) || compare_strings("~/", str, 0))
	{
		lst_node = ft_find_node(shell->variables, "HOME", 0, 1);
		if (!lst_node)
			return (getenv("HOME"));
		else
			return (lst_node->val);
	}
	else if (compare_strings("~+", str, 1) || compare_strings("~+/", str, 0))
		lst_node = ft_find_node(shell->variables, "PWD", 0, 1);
	else if (compare_strings("~-", str, 1) || compare_strings("~-/", str, 0))
		lst_node = ft_find_node(shell->variables, "OLDPWD", 0, 1);
	if (lst_node)
		return (lst_node->val);
	return (NULL);
}
