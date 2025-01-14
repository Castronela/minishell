/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   binaries.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pamatya <pamatya@student.42heilbronn.de    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/06/25 22:07:34 by pamatya           #+#    #+#             */
/*   Updated: 2025/01/14 03:50:14 by pamatya          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/minishell.h"

int			get_binaries(t_shell *shl);
char		*get_binary_path(t_cmds *cmd, char **env_paths);
int			remove_path(t_cmds *cmd);
static char	**get_env_paths(t_shell *shl);

/*
Function to get binaries
*/
int	get_binaries(t_shell *shl)
{
	t_cmds	*cmd;
	char	**env_paths;
	
	cmd = shl->cmds_lst;
	env_paths = get_env_paths(shl);
	while (cmd)
	{
		if (env_paths == NULL)
			cmd->bin_path = NULL;
		else if (env_paths && cmd->cmd_index != 0)
		{
			if (cmd->args)
				cmd->bin_path = get_binary_path(cmd, env_paths);
			if (!cmd->bin_path)
				exit_early(shl, env_paths, "Path malloc failed");
			if (remove_path(cmd) == -1)
				exit_early(shl, env_paths, "Remove-path malloc failed");	
		}
		cmd = cmd->next;
	}
	ft_free2d(env_paths);
	return (0);
}

/*
Function that returns a double char of paths malloc'd by ft_split by finding
the path value on shl->variables
*/
static char	**get_env_paths(t_shell *shl)
{
	char		**env_paths;
	t_lst_str	*paths;

	env_paths = NULL;
	paths = ft_find_node(shl->variables, "PATH", 0, 1);
	if (!paths)
		return (NULL);
	else
	{
		env_paths = ft_split(paths->val, ':');
		if (!env_paths)
			exit_early(shl, NULL, ERRMSG_MALLOC);
		return (env_paths);
	}
}

char	*get_binary_path(t_cmds *cmd, char **env_paths)
{
	char	*tmp[3];
	char	*cmd_name;
	int		i;

	cmd_name = *(cmd->args + cmd->skip);
	if (!(tmp[0] = ft_strdup(cmd_name)))
		return (perror("ft_strdup malloc failed:"), NULL);
	if ((access(tmp[0], F_OK) == 0 && tmp[0][0] == '/'))
		return (tmp[0]);
	i = -1;
	while (env_paths[++i])
	{
		if (!(tmp[1] = ft_strjoin(env_paths[i], "/")))
			return (free(tmp[0]), perror("tmp1 malloc failed:"), NULL);
		if (!(tmp[2] = ft_strjoin(tmp[1], cmd_name)))
			return (free(tmp[0]), free(tmp[1]), perror("tmp2 malloc failed:"), NULL);
		free(tmp[1]);
		if (access(tmp[2], F_OK) == 0)
			return (free(tmp[0]), tmp[2]);
		free(tmp[2]);
	}
	return (tmp[0]);
}

int	remove_path(t_cmds *cmd)
{
	char	*cmd_with_path;
	char	*cmd_wo_path;
	int		cmdlen[2];
	int		i;

	cmd_with_path = *(cmd->args + cmd->skip);
	cmdlen[0] = ft_strlen(cmd_with_path);
	i = 0;
	cmdlen[1] = 0;
	while (cmd_with_path[i])
	{
		cmdlen[1]++;
		if (cmd_with_path[i] == '/')
			cmdlen[1] = 0;
		i++;
	}
	cmd_wo_path = malloc((cmdlen[1] + 1) * sizeof(char));
	if (!cmd_wo_path)
		return (-1);
	cmd_wo_path[cmdlen[1]] = '\0';
	while (cmdlen[1]--)
		*(cmd_wo_path + cmdlen[1]) = *(cmd_with_path + --cmdlen[0]);
	ft_free_safe((void **)(&(cmd->args[cmd->skip])));
	return (cmd->args[cmd->skip] = cmd_wo_path, 0);
}
