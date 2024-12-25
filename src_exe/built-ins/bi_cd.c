/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   bi_cd.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pamatya <pamatya@student.42heilbronn.de    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/26 14:42:30 by pamatya           #+#    #+#             */
/*   Updated: 2024/12/25 17:11:23 by pamatya          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/minishell.h"

void	mini_cd(t_shell *shl, t_cmds *cmd);
int		path_is_dir(char *path);
void	update_cwd(t_shell *shl, char *new_cwd);

/*
Built-in cd function
  - Checks first if the path provided is just ".", in which case it does nothing
  - If the path is only "..", then cwd_up() is called which updates the cwd system variable to one step above
  - If the path contains anything else, it checks whether the path is valid and then updates the cwd system variable with the current provided path including necessary expansions from "." or ".." present within the path
*/
void	mini_cd(t_shell *shl, t_cmds *cmd)
{
	char		*new_cwd;
	t_lst_str	*node;
	
	if (*(cmd->args + 1) == NULL)
	{
		node = ft_find_node(shl->variables, "HOME", 0, 1);
		new_cwd = node->val;
	}	
	else if (compare_strings(*(cmd->args + 1), "-", 1))
	{
		node = ft_find_node(shl->variables, "OLDPWD", 0, 1);
		new_cwd = node->val;
	}
	else
		new_cwd = *(cmd->args + 1);
	if (chdir(new_cwd) < 0)
	{
		perror("Chdir error");
		shl->exit_code = errno;
	}
	new_cwd = getcwd(NULL, 0);
	update_cwd(shl, new_cwd);
	set_prompt(shl, "<< ", " >> % ");
}

/*
Function to check whether the given path is a valid path or not
  - Returns 1 if it is a valid path
  - Returns 0 if the path is invalid, or if the path results in a file
*/
int	path_is_dir(char *path)
{
	int			i;
	struct stat	bufr;
	
	i = stat(path, &bufr);
	if (i < 0)
		return (0);
	else if (i == 0)
	{
		if (S_ISREG(bufr.st_mode))
			return (0);
		else if (S_ISDIR(bufr.st_mode))
			return (1);
	}
	return (0);
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
	char		*old_pwd;

	env_node[0] = ft_find_node(shl->env, "PWD=", 0, 0);
	env_node[1] = ft_lst_new(ft_strjoin("PWD=", new_cwd), NULL);
	if (!env_node[1])
		exit_early(shl, NULL, "new_env_node malloc failed");
	ft_replace_node(env_node[0], env_node[1]);
	var_node[0] = ft_find_node(shl->variables, "PWD", 0, 1);
	var_node[1] = ft_lst_new("PWD", new_cwd);
	if (!var_node[1])
	{
		free(env_node[1]);
		exit_early(shl, NULL, "new_env_node malloc failed");
	}
	ft_replace_node(var_node[0], var_node[1]);
	free(shl->cur_wd);
	shl->cur_wd = ft_strdup(new_cwd);
}
