/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   binaries.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pamatya <pamatya@student.42heilbronn.de    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/06/25 22:07:34 by pamatya           #+#    #+#             */
/*   Updated: 2025/01/26 15:29:29 by pamatya          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/minishell.h"

void		set_binaries(t_shell *shl, t_cmds *cmd);
void		set_env_paths(t_shell *shl);
int			is_path(const t_cmds *cmd);

static void	set_binary_from_path(t_shell *shl, t_cmds *cmd);
static char	*get_assembled_bin_path(t_shell *shl, t_cmds *cmd,
				const char *env_path);

void	set_binaries(t_shell *shl, t_cmds *cmd)
{
	if (cmd->exit_code || is_built_in(cmd))
		return ;
	if (!is_path(cmd) && shl->env_paths)
		set_binary_from_path(shl, cmd);
	else if (cmd->args)
	{
		cmd->bin_path = ft_strdup(*(cmd->args + cmd->skip));
		if (!cmd->bin_path)
			exit_early(shl, NULL, ERRMSG_MALLOC);
		if (access(cmd->bin_path, F_OK | X_OK) < 0)
		{
			ft_fprintf_str(STDERR_FILENO, (const char *[]){ERSHL, cmd->bin_path,
				": ", strerror(errno), "\n", NULL});
			if (errno == ENOENT)
				cmd->exit_code = ERRCODE_CMD_OR_FILE_NOT_FOUND;
			if (errno == EACCES)
				cmd->exit_code = ERRCODE_CMD_CNOT_EXEC;
		}
		else if (path_is_dir(cmd->bin_path))
		{
			ft_fprintf_str(STDERR_FILENO, (const char *[]){ERSHL, cmd->bin_path,
				ERRMSG_PATH_IS_DIR, NULL});
			cmd->exit_code = ERRCODE_CMD_CNOT_EXEC;
		}
	}
}

/*
0 - if no PATH, do checks
0 - if found in PATH, do checks
1 - if not found in PATH, skip checks
*/
static void	set_binary_from_path(t_shell *shl, t_cmds *cmd)
{
	size_t	index;

	if (!cmd->args)
		return ;
	index = -1;
	while (shl->env_paths[++index])
	{
		if (cmd->bin_path)
			free(cmd->bin_path);
		cmd->bin_path = get_assembled_bin_path(shl, cmd, shl->env_paths[index]);
		if (!access(cmd->bin_path, F_OK) && !path_is_dir(cmd->bin_path))
			return ;
	}
	ft_fprintf_str(STDERR_FILENO, (const char *[]){ERSHL, *(cmd->args
			+ cmd->skip), ERRMSG_CMD_NOT_FOUND, NULL});
	cmd->exit_code = ERRCODE_CMD_OR_FILE_NOT_FOUND;
}

static char	*get_assembled_bin_path(t_shell *shl, t_cmds *cmd,
		const char *env_path)
{
	char	*bin_path;

	bin_path = ft_strdup(env_path);
	if (!bin_path)
		exit_early(shl, NULL, ERRMSG_MALLOC);
	if (bin_path[ft_strlen(bin_path) - 1] != '/')
	{
		if (append_to_str(&bin_path, "/", -1))
		{
			free(bin_path);
			exit_early(shl, NULL, ERRMSG_MALLOC);
		}
	}
	if (append_to_str(&bin_path, *(cmd->args + cmd->skip), -1))
	{
		free(bin_path);
		exit_early(shl, NULL, ERRMSG_MALLOC);
	}
	return (bin_path);
}

/*
Function that returns a double char of paths malloc'd by ft_split by finding
the path value on shl->variables
*/
void	set_env_paths(t_shell *shl)
{
	t_lst_str	*paths;

	if (shl->env_paths)
		ft_free2d(shl->env_paths);
	paths = ft_find_node(shl->variables, "PATH", 0, 1);
	if (!paths)
		shl->env_paths = NULL;
	else
	{
		shl->env_paths = ft_split(paths->val, ':');
		if (!shl->env_paths)
			exit_early(shl, NULL, ERRMSG_MALLOC);
	}
}

int	is_path(const t_cmds *cmd)
{
	char 	*cmd_name;
	size_t 	index;

	if (!cmd->args)
		return (0);
	cmd_name = *(cmd->args + cmd->skip);
	index = -1;
	while (cmd_name[++index])
	{
		if (cmd_name[index] == '/')
			return (1);
	}
	return (0);
}
