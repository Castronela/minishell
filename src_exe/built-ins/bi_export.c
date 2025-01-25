/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   bi_export.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: david <david@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/26 14:44:28 by pamatya           #+#    #+#             */
/*   Updated: 2025/01/25 04:11:02 by david            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/minishell.h"

int		mini_export(t_shell *shl, t_cmds *cmd);

static void			print_quoted_env(t_shell *shl);
static t_lst_str 	*get_next_var_alphabetically(t_shell *shl, t_lst_str *prev);
static void ft_lst_mv_node(t_lst_str **old_root, t_lst_str *node, 
	t_lst_str **new_root);
static void 		store_var(t_shell *shl, const char *arg);
static void 		update_var_node(t_shell *shl, char *var_name, char *var_value,
	int append);
static void 		add_new_var_node(t_shell *shl, char *var_name, char *var_value);
static char 		*get_export_var_name(t_shell *shl, const char *arg, int *append);
static char 		*get_export_var_value(t_shell *shl, const char *arg,
	char *var_name);

/*
Function for built-in export function
  - Adds the exported variable to the shl->env
  - Also adds the variable as key-val pair to the variables list

Note:	Here, the upd_str is only freed if checks[2] == 1 which indicates that
		it is a malloc'd string, otherwise it is simply a borrowed pointer
*/
int	mini_export(t_shell *shl, t_cmds *cmd)
{
	char	**arguments;
	size_t	index;

	arguments = cmd->args + cmd->skip;
	if (!arguments[1])
		print_quoted_env(shl);
	else
	{	
		index = 0;
		while (arguments[++index])
			store_var(shl, arguments[index]);
	}
	return (0);
}

static void store_var(t_shell *shl, const char *arg)
{
	int		append;
	char 	*var_name;
	char 	*var_value;

	append = 0;
	var_name = get_export_var_name(shl, arg, &append);
	if (!var_name)
	{
		if ((ft_fprintf_str(STDERR_FILENO, (const char *[]){ERSHL, "export: `",
					arg, ERRMSG_NO_VALID_IDENT, NULL})) < 0)
			exit_early(shl, NULL, ERRMSG_WRITE);
		return ;
	}
	var_value = get_export_var_value(shl, arg, var_name);
	update_var_node(shl, var_name, var_value, append);
}

static void ft_lst_mv_node(t_lst_str **old_root, t_lst_str *node, 
	t_lst_str **new_root)
{
	t_lst_str *prev;
	t_lst_str *next;

	prev = node->prev;
	next = node->next;
	if (*old_root == node)
		*old_root = node->next;
	else
		prev->next = next;
	if (next)
		next->prev = prev;
	node->prev = NULL;
	node->next = NULL;
	ft_lst_addback(new_root, node);
}

static void update_var_node(t_shell *shl, char *var_name, char *var_value,
	int append)
{
	t_lst_str *var_node;
	
	var_node = ft_find_node(shl->variables, var_name, 0, 1);
	if (!var_node)
	{
		var_node = ft_find_node(shl->local_vars, var_name, 0, 1);
		if (var_node)
			ft_lst_mv_node(&shl->local_vars, var_node, &shl->variables);
		else
			return (add_new_var_node(shl, var_name, var_value));	
	}
	free(var_name);
	if (var_value && !append && var_node->val)
	{
		free(var_node->val);
		var_node->val = NULL;
	}
	if (var_value && append_to_str(&var_node->val, var_value, -1))
	{
		free(var_value);
		exit_early(shl, NULL, ERRMSG_MALLOC);
	}
	if (var_value)
		free(var_value);
}

static void add_new_var_node(t_shell *shl, char *var_name, char *var_value)
{
	t_lst_str *var_node;

	var_node = ft_lst_new(var_name, var_value);
	free(var_name);
	if (var_value)
		free(var_value);
	if (!var_node)
		exit_early(shl, NULL, ERRMSG_MALLOC);
	ft_lst_addback(&shl->variables, var_node);
}

static char *get_export_var_name(t_shell *shl, const char *arg, int *append)
{
	size_t index;
	char *var_name;
	
	if (!ft_isalpha(arg[0]) && arg[0] != '_')
		return (NULL);
	index = 0;
	while (ft_isalnum(arg[index]) || arg[index] == '_')
		index++;
	if (compare_strings("+=", &arg[index], 0))
		*append = 1;
	else if (arg[index] != '=' && arg[index] != 0)
		return (NULL);
	var_name = ft_substr(arg, 0, index);
	if (!var_name)
		exit_early(shl, NULL, ERRMSG_MALLOC);
	return (var_name);
}

static char *get_export_var_value(t_shell *shl, const char *arg,
	char *var_name)
{
	size_t index;
	char *var_value;

	index = 0;
	while (arg[index] && arg[index] != '=')
		index++;
	if (arg[index++] == 0)
		return (NULL);
	var_value = ft_substr(arg, index, ft_strlen(arg) - index);
	if (!var_value)
	{
		free(var_name);
		exit_early(shl, NULL, ERRMSG_MALLOC);
	}
	return (var_value);
}

static void	print_quoted_env(t_shell *shl)
{
	t_lst_str *node;

	node = NULL;
	while (1)
	{
		node = get_next_var_alphabetically(shl, node);
		if (!node)
			break ;
		if (node->val)
			printf("declare -x %s=\"%s\"\n", node->key, node->val);
		else
			printf("declare -x %s\n", node->key);
	}
}

static t_lst_str *get_next_var_alphabetically(t_shell *shl, t_lst_str *prev)
{
    t_lst_str *node;
    t_lst_str *next;

	next = NULL;
    node = shl->variables;
    while (node)
    {
        if (node->key
            && (!prev 
			|| ft_strncmp(node->key, prev->key, ft_strlen(prev->key) + 1) > 0)
            && (!next 
			|| ft_strncmp(node->key, next->key, ft_strlen(node->key) + 1) < 0))
        {
            next = node;
        }
        node = node->next;
    }
    return (next);
}
