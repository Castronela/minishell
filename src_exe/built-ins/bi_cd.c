/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   bi_cd.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pamatya <pamatya@student.42heilbronn.de    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/26 14:42:30 by pamatya           #+#    #+#             */
/*   Updated: 2025/01/07 00:36:12 by pamatya          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/minishell.h"

void		mini_cd(t_shell *shl, t_cmds *cmd);
void		update_wdirs(t_shell *shl, char *new_cwd);
static int	get_new_cwd(t_shell *shl, t_cmds *cmd, char **new_cwd);
// int		path_is_dir(char *path);

/*
Built-in cd function
  - Checks first if the path provided is just ".", in which case it does nothing
  - If the path is only "..", then cwd_up() is called which updates the cwd system variable to one step above
  - If the path contains anything else, it checks whether the path is valid and then updates the cwd system variable with the current provided path including necessary expansions from "." or ".." present within the path

!!! Oldpwd should only be added once the cd function is executed, otherwise it
	should be empty

!!! Replace the printf and ft_fprintf() fns here with ft_fprintf_str() fn

!!! Other issues present as side comments
*/
void	mini_cd(t_shell *shl, t_cmds *cmd)
{
	char		*new_cwd;

	update_env_var(shl, cmd, UNDERSCORE, NULL);
	if (get_new_cwd(shl, cmd, &new_cwd))
	{
		if (chdir(new_cwd) < 0)
		{
			printf("%s %s: %s\n", ERRMSG_CD, new_cwd, strerror(errno));
			shl->exit_code = errno;	// (errno =2, but bash gives errno =1)
			return ;
		}
		new_cwd = getcwd(NULL, 0);
		update_wdirs(shl, new_cwd);
		if (compare_strings(*(cmd->args + 1), "-", 1))
			printf("%s\n", shl->cur_wd);
		set_prompt(shl, "<< ", " >> % ");		
	}
	shl->exit_code = 0;
}

static int	get_new_cwd(t_shell *shl, t_cmds *cmd, char **new_cwd)
{
	t_lst_str	*node;

	if (*(cmd->args + 1) == NULL || compare_strings(*(cmd->args + 1), "--", 1))
	{
		node = ft_find_node(shl->variables, "HOME", 0, 1);
		*new_cwd = node->val;
	}	
	else if (compare_strings(*(cmd->args + 1), "-", 1))
	{
		node = ft_find_node(shl->variables, "OLDPWD", 0, 1);
		if (!node)
		{
			ft_fprintf(STDERR_FILENO, "minishell: cd: OLDPWD not set\n");
			return (0);	
		}
		else
			*new_cwd = node->val;
	}
	else
		*new_cwd = *(cmd->args + 1);
	return (1);
}

/*
Function to update env variables pwd and oldpwd
  - Updates

!!! Potential leak: eg. in case of fn store_as_variable(); if there is an
	internal error like failed malloc inside this function when called from a
	funciton that sends another locally malloc'd object as argument to
	to it (here: new_pwd), store_as_variable() will exit by calling exit_early()
	fn which is not able to free the new_pwd pointer. In this case, such fns
	that may receive malloc'd objects as argument but call exit_early need to
	be modified to return an exit code so that the calling function gets a
	chance to free its allocated objects before calling exit_early.
	This potential leak should be checked through the entire program code. 
*/
void	update_wdirs(t_shell *shl, char *new_cwd)
{
	char		*old_pwd;
	char		*new_pwd;

	old_pwd = ft_strjoin("OLDPWD=", shl->cur_wd);
	if (!old_pwd)
		exit_early(shl, NULL, ERRMSG_MALLOC);
	add_to_environ(shl, old_pwd);
	store_as_variable(shl, old_pwd);
	free(old_pwd);
	new_pwd = ft_strjoin("PWD=", new_cwd);
	if (!new_pwd)
		exit_early(shl, NULL, ERRMSG_MALLOC);
	add_to_environ(shl, new_pwd);
	store_as_variable(shl, new_pwd);
	free(new_pwd);
	shl->cur_wd = ft_find_node(shl->variables, "PWD", 0, 1)->val;
}

// /*
// Function to check whether the given path is a valid path or not
//   - Returns 1 if it is a valid path
//   - Returns 0 if the path is invalid, or if the path results in a file
// */
// int	path_is_dir(char *path)
// {
// 	int			i;
// 	struct stat	bufr;
	
// 	i = stat(path, &bufr);
// 	if (i < 0)
// 		return (0);
// 	else if (i == 0)
// 	{
// 		if (S_ISREG(bufr.st_mode))
// 			return (0);
// 		else if (S_ISDIR(bufr.st_mode))
// 			return (1);
// 	}
// 	return (0);
// }

