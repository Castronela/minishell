/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   init_shell.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pamatya <pamatya@student.42heilbronn.de    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/03 03:40:07 by pamatya           #+#    #+#             */
/*   Updated: 2024/09/05 04:55:01 by pamatya          ###   ########.fr       */
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

void	init_shell(t_shell *shl, char **envp)
{
	shl->env = NULL;
	shl->env_bak = NULL;
	shl->env_paths = NULL;
	shl->cur_wd = NULL;
	shl->prev_bin_path = NULL;
	shl->prompt = NULL;
	copy_env(shl, envp);
	copy_env_paths(shl, envp);
	update_shlvl(shl);
	shl->cur_wd = getcwd(NULL, 0);
	if (!shl->cur_wd)
		exit_early(shl, NULL, "getcwd");
	set_prompt(shl, "Mini-Schale: ", " $> ");
	

}
                                                                                                                                                                                                                                                                       
void	copy_env(t_shell *shl, char **envp)
{
	int 		i;
	t_lst_str	*new_node[2];
	
	i = -1;
	while (envp[++i])
	{
		new_node[0] = ft_lst_new(envp[i]);
		if (!new_node[0])
			exit_early(shl, NULL, "Could not malloc t_lst_str node");
		ft_lst_addback(&shl->env, new_node[0]);
		new_node[1] = ft_lst_new(envp[i]);
		if (!new_node[1])
			exit_early(shl, NULL, "Could not malloc t_lst_str node");
		ft_lst_addback(&shl->env_bak, new_node[1]);
	}
}

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

void	update_shlvl(t_shell *shl)
{
	t_lst_str	*new_node[2];
	char		*shlvl;
	// char		shlvl;

	new_node[0] = shl->env;
	new_node[1] = shl->env_bak;
	while (new_node[0])
	{
		if (ft_strncmp(new_node[0]->str, "SHLVL=", 6) == 0)
		{
			shl->shlvl = ft_atoi(new_node[0]->str + 6) + 1;
			// shlvl = ft_strdup(ft_itoa(shl->shlvl));
			shlvl = ft_itoa(shl->shlvl);
			if (!shlvl)
				exit_early(shl, NULL, "itoa failed");
			*(new_node[0]->str + 6) = *shlvl;
			*(new_node[1]->str + 6) = *shlvl;
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
	if (shl->env_bak != NULL)
		ft_lst_free(&shl->env_bak);
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
