/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   init_shell.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pamatya <pamatya@student.42heilbronn.de    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/03 03:40:07 by pamatya           #+#    #+#             */
/*   Updated: 2024/12/14 18:06:51 by pamatya          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/minishell.h"

void 	init_shell(t_shell *shl, char **envp);
void	copy_env(t_shell *shl, char **envp);
void	copy_env_paths(t_shell *shl, char **envp);
void	update_shlvl(t_shell *shl);
void	set_prompt(t_shell *shl, char *prefix, char *separator);
char	*assemble_prompt(char *prefix, char *cwd, char *separator);

void	exit_early(t_shell *shl, char **split, char *msg);
void	ft_print_lst(t_lst_str *root);

void	arg_error(char **av);
void	clearout(t_shell *shl);

/*
Initializes the elements of the shell struct "t_shell"
  - Copies the environment variables to the shell struct
  - Copies the PATH variable to the shell struct
  - Updates the SHLVL variable
  - Sets the current working directory
  - Sets the prompt
  - Frees all malloc's and exits the program if any of the above steps fail
*/
void	init_shell(t_shell *shl, char **envp)
{
	shl->env = NULL;
	shl->variables = NULL;
	shl->env_paths = NULL;
	shl->cur_wd = NULL;
	shl->last_bin_arg = NULL;
	shl->prompt = NULL;
	copy_env(shl, envp);
	copy_env_paths(shl, envp);
	update_shlvl(shl);
	shl->cur_wd = getcwd(NULL, 0);
	if (!shl->cur_wd)
		exit_early(shl, NULL, "getcwd");
	set_prompt(shl, "Your wish My command: ", " $> ");


}

/*
Copies the environment variables to the shell struct
  - Copies the environment variables into shl->env and shl->variables as lists
  - Frees allocations and exits the program if malloc fails
  - Lists are created using ft_lst_new and ft_lst_addback
  - Memories and errors are handled by exit_early function in case of failure
*/
void	copy_env(t_shell *shl, char **envp)
{
	int 		i;
	t_lst_str	*new_node;
	char		**split;
	
	i = -1;
	while (envp[++i])
	{
		new_node = ft_lst_new(envp[i]);
		if (!new_node)
			exit_early(shl, NULL, "Could not malloc t_lst_str node");
		ft_lst_addback(&shl->env, new_node);
		split = ft_split(envp[i], '=');
		if (!split)
			exit_early(shl, NULL, "Could not split for new variable");
		new_node = ft_var_new(*split[0], *split[1]);
		if (!new_node)
			exit_early(shl, split, "Could not malloc t_lst_str new_node");
		ft_lst_addback(&shl->variables, new_node);
		ft_free2d(split);
	}
}

/*
Copies env path variable into shell struct
  - Splits the PATH variable using ft_split into a char**
  - Copies the PATH variable into shl->env_paths as a list
  - Frees allocations and exits the program if malloc fails
  - List is created using ft_lst_new and ft_lst_addback
  - Memories and errors are handled by exit_early function in case of failure
  - Allocations by ft_split are freed using ft_free2d
*/
void	copy_env_paths(t_shell *shl, char **envp)
{
	int			i;
	char		**paths;
	t_lst_str	*new_node;

	paths = NULL;
	i = -1;
	while (envp[++i])
	{
		if (ft_strncmp(envp[i], "PATH=", 5) == 0)
		{
			paths = ft_split(envp[i] + 5, ':');
			if (!paths)
				exit_early(shl, NULL, "Could not split PATH");
			break ;
		}
	}
	i = -1;
	while (paths[++i])
	{
		new_node = ft_lst_new(paths[i]);
		if (!new_node)
			exit_early(shl, paths, "Could not malloc t_lst_str node");
		ft_lst_addback(&shl->env_paths, new_node);
	}
	ft_free2d(paths);
}

/*
Updates the SHLVL environment variable
  - Finds the SHLVL variable in the environment list using ft_strncmp
  - Assignes shl->shlvl as int by converting using ft_atoi
  - Updates the SHLVL variable in the shl->env and shl->variables lists by 1 as char*
  - Frees the new value of SHLVL
  - Frees the new

!! Correction required:
  - This function will fail when the shlvl goes into two digits because, while
	the shlvl is malloc'd correctly for any case including for two digits, the
	env and variables elements of shl are only updated with assignment at a single
	char address by dereferencing shlvl instead of replacing the allocation itself
*/
void	update_shlvl(t_shell *shl)
{
	t_lst_str	*new_node[2];
	char		*shlvl;
	// char		shlvl;

	new_node[0] = shl->env;
	new_node[1] = shl->variables;
	while (new_node[0])
	{
		if (ft_strncmp(new_node[0]->str, "SHLVL=", 6) == 0)
		{
			shl->shlvl = ft_atoi(new_node[0]->str + 6) + 1;
			// shlvl = ft_strdup(ft_itoa(shl->shlvl));
			shlvl = ft_itoa(shl->shlvl);
			if (!shlvl)
				exit_early(shl, NULL, "itoa failed");
			*(new_node[0]->str + 6) = *shlvl;										// Correction required
			*(new_node[1]->str + 6) = *shlvl;										// Correction required
			free(shlvl);
			break ;
		}
		new_node[0] = new_node[0]->next;
		new_node[1] = new_node[1]->next;
	}
}

void	set_prompt(t_shell *shl, char *prefix, char *separator)
{
	char	**split;
	int		i;

	split = ft_split(shl->cur_wd, '/');
	if (!split)
		exit_early(shl, NULL, "Could not split cwd");
	i = -1;
	while (split[++i])
	{
		if (split[i + 1] == NULL)
		{
			shl->prompt = assemble_prompt(prefix, split[i], separator);
			if (!shl->prompt)
				exit_early(shl, split, "Could not assemble prompt");
			break ;
		}
	}
	ft_free2d(split);
}

char	*assemble_prompt(char *prefix, char *cwd, char *separator)
{
	char	*tmp[2];

	tmp[0] = ft_strjoin(prefix, cwd);
	if (!tmp[0])
		return (NULL);
	tmp[1] = ft_strjoin(tmp[0], separator);
	if (!tmp[1])
		return (free(tmp[0]), NULL);
	free(tmp[0]);
	return (tmp[1]);
}

void	exit_early(t_shell *shl, char **split, char *msg)
{
	if (shl->env != NULL)
		ft_lst_free(&shl->env);
	if (shl->variables != NULL)
		ft_lst_free(&shl->variables);
	if (shl->env_paths != NULL)
		ft_lst_free(&shl->env_paths);
	if (shl->cur_wd)
		free(shl->cur_wd);
	if (shl->prompt)
		free(shl->prompt);
	if (split)
		ft_free2d(split);
	perror(msg);
	exit(errno);
}

void	ft_print_lst(t_lst_str *root)
{
	while (root)
	{
		ft_printf("%s\n", root->str);
		root = root->next;
	}
}

void	arg_error(char **av)
{
	// ft_fprintf(2, "Minishell: %s: No such file or directory\n", av[1]);
	ft_putstr_fd("Minishell: ", 2);
	ft_putstr_fd(av[1], 2);
	ft_putstr_fd(": No such file or directory\n", 2);
	exit(127);
}

void	clearout(t_shell *shl)
{
	ft_lst_free(&shl->env);
	ft_lst_free(&shl->variables);
	ft_lst_free(&shl->env_paths);
	free(shl->cur_wd);
	free(shl->prompt);
	// free(shl->last_bin_arg);
	rl_clear_history();
}
