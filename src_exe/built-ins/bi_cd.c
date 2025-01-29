/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   bi_cd.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: david <david@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/26 14:42:30 by pamatya           #+#    #+#             */
/*   Updated: 2025/01/30 01:09:58 by david            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/minishell.h"

void		mini_cd(t_shell *shl, t_cmds *cmd);

static int	get_new_cwd(t_shell *shl, char *arg, char **new_cwd);
static int	get_new_cwd_helper(t_shell *shl, char *arg, char **new_cwd);
static void	update_wdirs(t_shell *shl, char *new_cwd);

int			path_is_dir(char *path);

/*
Built-in cd function
  - Checks first if the path provided is just ".", in which case it does nothing
  - If the path is only "..",
	then cwd_up() is called which updates the cwd system variable to one step above
  - If the path contains anything else,
	it checks whether the path is valid and then updates the cwd system variable with the current provided path including necessary expansions from "." or ".." present within the path

!!! !! cd function should not add an entry of PWD after it is unset, until it is
		explicitly set by export
		But OLDPWD still cycles between the prev folder like usual even though
		PWD is not present in the environment; maybe this can be done using shl->cur_wd

!!! Oldpwd should only be added once the cd function is executed, otherwise it
	should be empty
-->> Done.

!!! PWD and OLDPWD once unset, should still work like normal but should not be
	added back to env or export unless done so explicitly using export command
*/
void	mini_cd(t_shell *shl, t_cmds *cmd)
{
	char	*new_cwd;
	char	*arg;

	update_env_var(shl, cmd, UNDERSCORE, NULL);
	arg = *(cmd->args + cmd->skip + 1);
	if (get_new_cwd(shl, arg, &new_cwd) == 0)
	{
		if (chdir(new_cwd) < 0)
		{
			ft_fprintf_str(STDERR_FILENO, (const char *[]){ERSHL, ERRMSG_CD,
				new_cwd, ": ", strerror(errno), "\n", NULL});
			cmd->exit_code = ERRCODE_GENERAL;
			return ;
		}
		new_cwd = getcwd(NULL, 0);
		update_wdirs(shl, new_cwd);
		free(new_cwd);
		if (compare_strings(arg, "-", 1) == 1)
			printf("%s\n", shl->cur_wd);
		free(shl->prompt);
		set_prompt(shl, "<< ", " >> % ");
	}
	else
		cmd->exit_code = ERRCODE_GENERAL;
}

// Helper static fn for mini_cd() fn
static int	get_new_cwd(t_shell *shl, char *arg, char **new_cwd)
{
	t_lst_str	*node;

	if (arg == NULL || compare_strings(arg, "--", 1))
	{
		node = ft_find_node(shl->variables, "HOME", 0, 1);
		if (!node)
			return (shl->exit_code = 1, ft_fprintf_str(STDERR_FILENO,
					(const char *[]){ERSHL, "cd: HOME not set\n", NULL}), -1);
		*new_cwd = node->val;
	}
	else
		if (get_new_cwd_helper(shl, arg, new_cwd) == -1)
			return (-1);
	return (shl->exit_code = 0, 0);
}

// Helper static function for get_new_cwd() fn
static int	get_new_cwd_helper(t_shell *shl, char *arg, char **new_cwd)
{
	t_lst_str	*node;
	
	if (compare_strings(arg, "-", 1))
	{
		node = ft_find_node(shl->variables, "OLDPWD", 0, 1);
		if (!node || !(node->val))
			return (shl->exit_code = 1, ft_fprintf_str(STDERR_FILENO,
					(const char *[]){ERSHL, "cd: OLDPWD not set\n", NULL}), -1);
		*new_cwd = node->val;
	}
	else if (compare_strings(arg, ".", 1))
		*new_cwd = shl->cur_wd;
	else
		*new_cwd = arg;
	return (0);
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
static void	update_wdirs(t_shell *shl, char *new_cwd)
{
	char		*old_pwd;
	char		*new_pwd;
	t_lst_str	*cur_pwd_node;

	cur_pwd_node = ft_find_node(shl->variables, "PWD", 0, 1);
	if (!cur_pwd_node)
		old_pwd = ft_strjoin("OLDPWD=", "");
	else
		old_pwd = ft_strjoin("OLDPWD=", cur_pwd_node->val);
	if (!old_pwd)
		exit_early(shl, NULL, ERRMSG_MALLOC);
	add_to_environ(shl, old_pwd, 0);
	store_as_variable(shl, old_pwd, 0);
	free(old_pwd);
	new_pwd = ft_strjoin("PWD=", new_cwd);
	if (!new_pwd)
		exit_early(shl, NULL, ERRMSG_MALLOC);
	add_to_environ(shl, new_pwd, 0);
	store_as_variable(shl, new_pwd, 0);
	free(new_pwd);
	if (shl->old_wd)
		free(shl->old_wd);
	shl->old_wd = shl->cur_wd;
	shl->cur_wd = getcwd(NULL, 0);
	if (!shl->cur_wd)
		exit_early(shl, NULL, ERRMSG_MALLOC);
}

/*
Function to check whether the given path is a valid path or not
  - Returns 1 if it is a valid path
  - Returns 0 if the path is invalid, or if the path results in a file

!!! Move this to a different file as it is not used within this file
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
