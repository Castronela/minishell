/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   init_shell.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pamatya <pamatya@student.42heilbronn.de    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/03 03:40:07 by pamatya           #+#    #+#             */
/*   Updated: 2025/01/03 20:53:05 by pamatya          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/minishell.h"

void 		init_shell(t_shell *shl, char **envp);
void		copy_environ_variables(t_shell *shl, char **envp);
static void	copy_environ(t_shell *shl, char **envp, int size);
void		copy_env_paths(t_shell *shl, char **envp);
void		update_shlvl(t_shell *shl);
void		set_prompt(t_shell *shl, char *prefix, char *separator);
char		*assemble_prompt(char *prefix, char *cwd, char *separator);

/*
Initializes the elements of the shell struct "t_shell"
  - Copies the environment variables to the shell struct
  - Copies the PATH variable to the shell struct
  - Updates the SHLVL variable
  - Sets the current working directory
  - Sets the prompt
  - Frees all malloc's and exits the program if any of the above steps fail

!!! Need to look into update_shlvl for leaks and other potential problems
-->>	Potentially totally fixed. Haven't checked with valgrind yet.
*/
void	init_shell(t_shell *shl, char **envp)
{
	shl->stdio[0] = dup(STDIN_FILENO);
	shl->stdio[1] = dup(STDOUT_FILENO);
	shl->environ = NULL;
	shl->variables = NULL;
	shl->env_paths = NULL;
	shl->shlvl = 1;
	shl->cur_wd = NULL;
	shl->prompt = NULL;
	copy_environ_variables(shl, envp);
	copy_env_paths(shl, envp);
	update_shlvl(shl);
	shl->cur_wd = getcwd(NULL, 0);
	if (!shl->cur_wd)
		exit_early(shl, NULL, "getcwd");
	set_prompt(shl, "<< ", " >> % ");
	shl->exit_code_prev = 0;
	shl->exit_code = 0;
	reset_cmd_vars(shl, 0);
}

/*
Copies the environment variables to the shell struct
  - Copies the environment variables into shl->env and shl->variables as lists
  - Frees allocations and exits the program if malloc fails
  - Lists are created using ft_lst_new and ft_lst_addback
  - Memories and errors are handled by exit_early function in case of failure
*/
void	copy_environ_variables(t_shell *shl, char **envp)
{
	int 		i;
	t_lst_str	*new_node;
	char		**split;

	i = -1;
	while (envp[++i])
	{
		split = ft_split(envp[i], '=');
		if (!split)
			exit_early(shl, NULL, "Could not split for new variable");
		new_node =ft_lst_new(split[0], split[1]);
		if (!new_node)
			exit_early(shl, split, "Could not malloc t_lst_str new_node");
		ft_lst_addback(&shl->variables, new_node);
		ft_free2d(split);
	}
	copy_environ(shl, envp, i);
}

/*
Function to copy envp variables as double char pointers, as required by execve
*/
static void	copy_environ(t_shell *shl, char **envp, int size)
{
	int i;
	
	shl->environ = malloc((size + 1) * sizeof(char *));
	if (!shl->environ)
		exit_early(shl, NULL, ERRMSG_MALLOC);
	i = -1;
	while (envp[++i])
	{
		shl->environ[i] = ft_strdup(envp[i]);
		if (!shl->environ[i])
			exit_early(shl, NULL, ERRMSG_MALLOC);
	}
	shl->environ[i] = NULL;
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
		new_node = ft_lst_new(paths[i], NULL);
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

!!! Note:
  - Although this fn has been rebuilt, needs to be checked with valgrind for 
  	memory leaks
*/
void	update_shlvl(t_shell *shl)
{
	char		*shlvl;
	int			i;
	t_lst_str	*var_node[2];
	
	i = -1;
	while (shl->environ[++i])
	{
		if (compare_strings("SHLVL=", shl->environ[i], 0))
		{
			shlvl = shl->environ[i] + 6;
			break ;
		}
	}
	shl->shlvl += ft_atoi(shlvl);
	shlvl = ft_strjoin("SHLVL=", ft_itoa(shl->shlvl));
	if (!shlvl)
		exit_early(shl, NULL, ERRMSG_MALLOC);
	free(shl->environ[i]);
	shl->environ[i] = shlvl;
	var_node[0] = ft_find_node(shl->variables, "SHLVL", 0, 1);
	var_node[1] = ft_lst_new("SHLVL", ft_itoa(shl->shlvl));
	if (!var_node[1])
		exit_early(shl, NULL, ERRMSG_MALLOC);
	ft_replace_node(shl, &(var_node[0]), var_node[1]);
	var_node[0] = ft_find_node(shl->variables, "SHLVL", 0, 1);
	printf("%s:	%s\n", var_node[0]->key, var_node[0]->val);
}

/*
Function to set-up the prompt of minishell
*/
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
