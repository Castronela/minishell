/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   init_shell.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pamatya <pamatya@student.42heilbronn.de    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/03 03:40:07 by pamatya           #+#    #+#             */
/*   Updated: 2024/09/04 04:20:40 by pamatya          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/minishell.h"

void 	init_shell(t_shell *shl, char **envp);
void	copy_env(t_shell *shl, char **envp);
void	copy_env_paths(t_shell *shl, char **envp);
void	update_shlvl(t_shell *shl, char **envp);
// void	update_cwd(t_shell *shl);

void	exit_early(t_shell *shl, char **split2free, char *msg);
void	ft_print_lst(t_lst_str *root);

void	init_shell(t_shell *shl, char **envp)
{
	shl->env = NULL;
	shl->env_bak = NULL;
	copy_env(shl, envp);
	shl->env_paths = NULL;
	copy_env_paths(shl, envp);
	update_shlvl(shl, envp);
	// set_env_params(shl);
	
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

void	update_shlvl(t_shell *shl, char **envp)
{
	int	i;
	int	shlvl;

	i = -1;
	shlvl = 1;
	while (envp[++i])
	{
		if (ft_strncmp(envp[i], "SHLVL=", 6) == 0)
		{
			shlvl = ft_atoi(envp[i] + 6);
			break ;
		}
	}
	shl->shlvl = shlvl + 1;
}


void	exit_early(t_shell *shl, char **split2free, char *msg)
{
	if (shl->env != NULL)
		ft_lst_free(&shl->env);
	if (shl->env_bak != NULL)
		ft_lst_free(&shl->env_bak);
	if (shl->env_paths != NULL)
		ft_lst_free(&shl->env_paths);
	if (split2free)
		ft_free2d(split2free);
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
