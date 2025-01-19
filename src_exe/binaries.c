/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   binaries.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: david <david@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/06/25 22:07:34 by pamatya           #+#    #+#             */
/*   Updated: 2025/01/19 04:31:31 by david            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/minishell.h"

void 	set_binaries(t_shell *shl, t_cmds *cmd);
void	set_env_paths(t_shell *shl);
int		is_path(const t_cmds *cmd);

static void set_binary_from_path(t_shell *shl, t_cmds *cmd);
static char *get_assembled_bin_path(t_shell *shl, t_cmds *cmd, const char *env_path);

void set_binaries(t_shell *shl, t_cmds *cmd)
{
	if (cmd->exit_code || !cmd->exc_index)
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
	}
}

/*
0 - if no PATH, do checks
0 - if found in PATH, do checks
1 - if not found in PATH, skip checks
*/
static void set_binary_from_path(t_shell *shl, t_cmds *cmd)
{
	size_t index;

	if (!cmd->args)
		return ;
	index = -1;
	while (shl->env_paths[++index])
	{
		if (cmd->bin_path)
			free(cmd->bin_path);
		cmd->bin_path = get_assembled_bin_path(shl, cmd, shl->env_paths[index]);
		if (!access(cmd->bin_path, F_OK))
			return ;
	}
	cmd->bin_path = ft_strdup(*(cmd->args + cmd->skip));
	if (!cmd->bin_path)
		exit_early(shl, NULL, ERRMSG_MALLOC);
}

static char *get_assembled_bin_path(t_shell *shl, t_cmds *cmd, 
	const char *env_path)
{
	char *bin_path;
	
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
Function to get binaries
*/
// int	get_binary(t_shell *shl, t_cmds *cmd)
// {
// 	char	**env_paths;
// 	int		cmd_not_found;

// 	env_paths = get_env_paths(shl);
// 	if (cmd->cmd_index != 0)
// 	{
// 		// if (!env_paths)
// 		// 	if (!(cmd->bin_path = ft_strdup("")))
// 		// 		exit_early(shl, env_paths, "bin-path malloc failed");
				
// 		// if (env_paths && cmd->args)
// 		if (cmd->args)
// 		{
// 			cmd_not_found = get_binary_path(cmd, env_paths);
// 			if (cmd_not_found == -1)
// 				exit_early(shl, env_paths, "Path malloc failed");
// 			if (cmd_not_found)
// 				exit_early(shl, NULL, NULL);
// 		}
// 		if (remove_path(cmd) == -1)
// 			exit_early(shl, env_paths, "Remove-path malloc failed");
// 	}	
// 	if (env_paths)
// 		ft_free2d(env_paths);
// 	return (0);
// }

/*
Function that returns a double char of paths malloc'd by ft_split by finding
the path value on shl->variables
*/
void	set_env_paths(t_shell *shl)
{
	t_lst_str	*paths;

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

// static int	get_binary_path(t_cmds *cmd, char **env_paths)
// {
// 	char	*tmp[3];
// 	char	*cmd_name;
// 	int		i;

// 	cmd_name = *(cmd->args + cmd->skip);
// 	tmp[0] = ft_strdup(cmd_name);
// 	if (!tmp[0])
// 		return (-1);
// 	// if ((access(tmp[0], F_OK) == 0 && (tmp[0][0] == '/' || tmp[0][0] == '.')))
// 	// if ((is_path(tmp[0]) && access(tmp[0], F_OK) == 0))
// 	if (is_path(tmp[0]) || !env_paths)
// 	{
// 		cmd->bin_path = tmp[0];
// 		return (0);
// 	}
// 	i = -1;
// 	while (env_paths && env_paths[++i])
// 	{
// 		if (!(tmp[1] = ft_strjoin(env_paths[i], "/")))
// 			return (free(tmp[0]), perror("tmp1 malloc failed:"), -1);
// 		if (!(tmp[2] = ft_strjoin(tmp[1], cmd_name)))
// 			return (free(tmp[0]), free(tmp[1]), perror("tmp2 malloc failed:"), -1);
// 		free(tmp[1]);
// 		if (access(tmp[2], F_OK) == 0)
// 		{
// 			cmd->bin_path = tmp[2];
// 			free(tmp[0]);
// 			return (0);
// 		}
// 		free(tmp[2]);
// 	}
// 	ft_fprintf_str(STDERR_FILENO, (const char *[]){"minishell: ", 
// 			*(cmd->args + cmd->skip), ": command not found\n", NULL});
	
// 	// if (!env_paths)
// 	// {
// 	// 	tmp[1] = ft_strdup("");
// 	// 	if (!tmp[1])
// 	// 		return (free(tmp[0]), NULL);
// 	// 	return (free(tmp[0]), tmp[1]);
// 	// }
// 	return (1);
// }

/*
Scenario 1:
	- arg IS a path
	- do nothing
	- execve handles error message
Scenario 2:
	- arg is NOT path && arg not found in PATH
	=> error command not found
Scenario 3:
	- arg is NOT a path && PATH is NULL
	- do nothing
	- execve handles error message

*/
// static int	remove_path(t_cmds *cmd)
// {
// 	char	*cmd_with_path;
// 	char	*cmd_wo_path;
// 	int		cmdlen[2];
// 	int		i;

// 	cmd_with_path = *(cmd->args + cmd->skip);
// 	cmdlen[0] = ft_strlen(cmd_with_path);
// 	i = 0;
// 	cmdlen[1] = 0;
// 	while (cmd_with_path[i])
// 	{
// 		cmdlen[1]++;
// 		if (cmd_with_path[i] == '/')
// 			cmdlen[1] = 0;
// 		i++;
// 	}
// 	cmd_wo_path = malloc((cmdlen[1] + 1) * sizeof(char));
// 	if (!cmd_wo_path)
// 		return (-1);
// 	cmd_wo_path[cmdlen[1]] = '\0';
// 	while (cmdlen[1]--)
// 		*(cmd_wo_path + cmdlen[1]) = *(cmd_with_path + --cmdlen[0]);
// 	ft_free_safe((void **)(&(cmd->args[cmd->skip])));
// 	return (cmd->args[cmd->skip] = cmd_wo_path, 0);
// }

int	is_path(const t_cmds *cmd)
{
	char	*cmd_name;
	size_t	index;
	
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