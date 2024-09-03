/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   init_shell.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pamatya <pamatya@student.42heilbronn.de    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/03 03:40:07 by pamatya           #+#    #+#             */
/*   Updated: 2024/09/03 04:22:57 by pamatya          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/minishell.h"

void 		init_shell(t_shell *shl, char **envp);
void		copy_envp(t_shell *shl, char **envp);
void		ft_print_lst(t_lst_str *root);
static void	handle_err_and_exit(t_shell *shl, char *str);

void	init_shell(t_shell *shl, char **envp)
{
	// int i;
	
	shl->env = NULL;
	shl->env_orig_bak = NULL;
	copy_envp(shl, envp);


	
}

void	copy_envp(t_shell *shl, char **envp)
{
	int 		i;
	t_lst_str	*new_node[2];
	
	i = -1;
	while (envp[++i])
	{
		new_node[0] = ft_lst_new(envp[i]);
		if (!new_node[0])
			handle_err_and_exit(shl, "Could not malloc t_lst_str node");
		new_node[1] = ft_lst_new(envp[i]);
		if (!new_node[1])
			handle_err_and_exit(shl, "Could not malloc t_lst_str bak_node");
		ft_lst_addback(&shl->env, new_node[0]);
		ft_lst_addback(&shl->env_orig_bak, new_node[1]);
	}
}

void	ft_print_lst(t_lst_str *root)
{
	while (root)
	{
		ft_printf("%s\n,", root->str);
		root = root->next;
	}
}

static void	handle_err_and_exit(t_shell *shl, char *str)
{
	ft_lst_free(&shl->env);
	ft_lst_free(&shl->env_orig_bak);
	perror(str);
	exit(errno);
}
