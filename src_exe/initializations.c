/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   initializations.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pamatya <pamatya@student.42heilbronn.de    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/03 03:40:07 by pamatya           #+#    #+#             */
/*   Updated: 2025/01/26 13:45:26 by pamatya          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/minishell.h"

void		init_environ_variables(t_shell *shl, char **envp);
void		update_shlvl(t_shell *shl);
void		set_prompt(t_shell *shl, char *prefix, char *separator);

static void	copy_environ(t_shell *shl, char **envp, int size);
static char	*assemble_prompt(char *prefix, char *cwd, char *separator);


/*
Copies the environment variables to the shell struct
  - Copies the environment variables into shl->env and shl->variables as lists
  - Frees allocations and exits the program if malloc fails
  - Lists are created using ft_lst_new and ft_lst_addback
  - Memories and errors are handled by exit_early function in case of failure
  
!!! Here it is required to be 'i' and not 'i - 1' (side annotation below)
-->>	Because when OLDPWD is not already there, i for OLDPWD will not be 
		skipped, and so the 'i' passed in this case to copy_environ() fn will be
		one less than the number of env vars to be copied. This will result in a
		seg-fault.
*/
void	init_environ_variables(t_shell *shl, char **envp)
{
	int 		i;
	t_lst_str	*new_node;
	char		**sp;

	i = -1;
	while (envp[++i])
	{
		if (ft_strncmp(envp[i], "OLDPWD=", 7) != 0)
		{
			sp = ft_split(envp[i], '=');
			if (!sp)
				exit_early(shl, NULL, ERRMSG_MALLOC);
			new_node = ft_lst_new(sp[0], (envp[i] + var_offset(envp[i], 1)));
			ft_free2d(sp);
		}
		else
			new_node = ft_lst_new("OLDPWD", NULL);
		if (!new_node)
			exit_early(shl, NULL, ERRMSG_MALLOC);
		ft_lst_addback(&shl->variables, new_node);
	}
	copy_environ(shl, envp, i);		// Read comment above on 'i' or 'i - 1'
	shl->home_dir = ft_strdup(ft_find_node(shl->variables, "HOME", 0, 1)->val);
	if (!shl->home_dir)
		exit_early(shl, NULL, ERRMSG_MALLOC);
}

// void	init_environ_variables(t_shell *shl, char **envp)
// {
// 	int 		i;
// 	t_lst_str	*new_node;
// 	char		**split;

// 	i = -1;
// 	while (envp[++i])
// 	{
// 		if (ft_strncmp(envp[i], "OLDPWD=", 7) != 0)
// 		{
// 			split = ft_split(envp[i], '=');
// 			if (!split)
// 				exit_early(shl, NULL, "Could not split for new variable");
// 			new_node =ft_lst_new(split[0], split[1]);
// 			if (!new_node)
// 				exit_early(shl, split, "Could not malloc t_lst_str new_node");
// 			ft_lst_addback(&shl->variables, new_node);
// 			ft_free2d(split);
// 		}
// 	}
// 	copy_environ(shl, envp, i);
// }

/*
Function to copy envp variables as double char pointers (as required by execve)
*/
static void	copy_environ(t_shell *shl, char **envp, int size)
{
	int i;
	int	j;
	
	shl->environ = malloc((size + 1) * sizeof(char *));
	if (!shl->environ)
		exit_early(shl, NULL, ERRMSG_MALLOC);
	i = -1;
	j = 0;
	while (envp[++i])
	{
		if (ft_strncmp(envp[i], "OLDPWD=", 7) != 0)
		{
			shl->environ[j] = ft_strdup(envp[i]);
			if (!shl->environ[j])
				exit_early(shl, NULL, ERRMSG_MALLOC);
			j++;
		}
	}
	shl->environ[j] = NULL;
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
	int			index;
	char		*tmp;
	
	index = find_dptr_index(shl, "SHLVL=", 6);
	if (index < 0)
		return ;
	shlvl = shl->environ[index] + 6;
	shl->shlvl += ft_atoi(shlvl);
	if (shl->shlvl < 0)
		shl->shlvl = 0;
	tmp = ft_itoa(shl->shlvl);
	if (!tmp)
		exit_early(shl, NULL, ERRMSG_MALLOC);
	shlvl = ft_strjoin("SHLVL=", tmp);
	if (!shlvl)
	{
		free(tmp);
		exit_early(shl, NULL, ERRMSG_MALLOC);
	}
	free(shl->environ[index]);
	shl->environ[index] = shlvl;
	store_as_variable(shl, shl->environ[index], 0);
	free(tmp);
}

// void	update_shlvl_old(t_shell *shl)
// {
// 	char		*shlvl;
// 	int			i;
// 	t_lst_str	*var_node[2];
// 	char		*tmp;
	
// 	i = -1;
// 	while (shl->environ[++i])
// 	{
// 		if (compare_strings("SHLVL=", shl->environ[i], 0))
// 		{
// 			shlvl = shl->environ[i] + 6;
// 			break ;
// 		}
// 	}
// 	shl->shlvl += ft_atoi(shlvl);
// 	tmp = ft_itoa(shl->shlvl);
// 	if (!tmp)
// 		exit_early(shl, NULL, ERRMSG_MALLOC);
// 	shlvl = ft_strjoin("SHLVL=", tmp);
// 	if (!shlvl)
// 	{
// 		free(tmp);
// 		exit_early(shl, NULL, ERRMSG_MALLOC);
// 	}
// 	free(shl->environ[i]);
// 	shl->environ[i] = shlvl;
// 	var_node[0] = ft_find_node(shl->variables, "SHLVL", 0, 1);
// 	var_node[1] = ft_lst_new("SHLVL", tmp);
// 	if (!var_node[1])
// 	{
// 		free(tmp);
// 		exit_early(shl, NULL, ERRMSG_MALLOC);
// 	}
// 	ft_replace_node(shl, &(var_node[0]), var_node[1]);
// 	var_node[0] = ft_find_node(shl->variables, "SHLVL", 0, 1);
// 	free(tmp);
// 	printf("%s:	%s\n", var_node[0]->key, var_node[0]->val);
// }

/*
Function to set-up the prompt of minishell
*/
void	set_prompt(t_shell *shl, char *prefix, char *separator)
{
	char	**split;
	int		i;

	split = ft_split(shl->cur_wd, '/');
	if (!split)
		exit_early(shl, NULL, ERRMSG_MALLOC);
	i = -1;
	while (split[++i])
	{
		if (split[i + 1] == NULL)
		{
			shl->prompt = assemble_prompt(prefix, split[i], separator);
			if (!shl->prompt)
				exit_early(shl, split, ERRMSG_MALLOC);
			break ;
		}
	}
	if (split[0] == NULL)
		shl->prompt = assemble_prompt(prefix, shl->cur_wd, separator);
	ft_free2d(split);
}

static char	*assemble_prompt(char *prefix, char *cwd, char *separator)
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
