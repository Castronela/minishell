/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   environmentalists.c                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pamatya <pamatya@student.42heilbronn.de    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/26 16:06:39 by pamatya           #+#    #+#             */
/*   Updated: 2024/12/30 18:47:53 by pamatya          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/minishell.h"

void	update_env_var(t_shell *shl, t_cmds *cmd);
void	update_cwd(t_shell *shl, char *new_cwd);
void	store_variable(t_shell *shl, char *str);
void	add_to_environment(t_shell *shl, t_cmds *cmd);

/*
Function to update value of $_ variable in all places
  - shl->environ
  - shl->env
  - shl->variables
*/
void	update_env_var(t_shell *shl, t_cmds *cmd)
{
	char		**env;
	t_lst_str	*env_lst[2];

	env = find_string_ptr(shl->environ, "_=", 2);
	if (update_var_str(env, "_=", cmd->bin_path) == -1)
		exit_early(shl, NULL, ERRMSG_MALLOC);

	env_lst[0] = ft_find_node(shl->variables, "_", 0, 1);
	printf("Found node:\n");
	printf("	env_lst[0]->key:	%s\n", env_lst[0]->key);
	printf("	env_lst[0]->val:	%s\n", env_lst[0]->val);
	env_lst[1] = ft_lst_new("_", cmd->args[count_pointers(cmd->args) - 1]);
	ft_replace_node(&(env_lst[0]), env_lst[1]);
}

/*
Function to update the current working directory
  - Updates 


!!! Potential leaks when not freeing the pointer returned by ft_strjoin
*/
void	update_cwd(t_shell *shl, char *new_cwd)
{
	t_lst_str	*env_node[4];
	t_lst_str	*var_node[4];
	// char		*old_pwd;

	var_node[0] = ft_find_node(shl->variables, "PWD", 0, 1);
	var_node[1] = ft_lst_new("PWD", new_cwd);
	if (!var_node[1])
	{
		free(env_node[1]);
		exit_early(shl, NULL, "new_env_node malloc failed");
	}
	ft_replace_node(&(var_node[0]), var_node[1]);
	free(shl->cur_wd);
	shl->cur_wd = ft_strdup(new_cwd);
}

/*
Function to add a new varaible to the minishell memory but not to shl->env
  - The parameter 'str' is split against '=' and ported intoft_lst_new fn
  - One variable is added upon per fn call
  - Does not write the variable to shl->env, this is only possible using the
    mini_export function
  - If malloc fails at any time, exit_early function is used to exit safely
*/
void	store_variable(t_shell *shl, char *str)
{
	t_lst_str	*new_var;
	char		**split;

	split = ft_split(str, '=');
	if (!split)
		exit_early(shl, NULL, "Could not split new variable string");
	new_var =ft_lst_new(*split, *(split + 1));
	if (!new_var)
		exit_early(shl, split, "Could not malloc new variable list node");
	ft_lst_addback(&shl->variables, new_var);
	ft_free2d(split);
}

/*
Origin file: bi_export.c
*/
void	add_to_environment(t_shell *shl, t_cmds *cmd)
{
	size_t		dp_len;
	
	dp_len = count_pointers(shl->environ);
	shl->environ = ft_recalloc(shl->environ, (dp_len + 2) * 
			sizeof(*shl->environ), (dp_len + 1) * sizeof(*shl->environ));
	shl->environ[dp_len] = *(cmd->args + 1);
	shl->environ[dp_len + 1] = NULL;
}