/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   binaries.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pamatya <pamatya@student.42heilbronn.de    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/06/25 22:07:34 by pamatya           #+#    #+#             */
/*   Updated: 2025/01/11 18:43:54 by pamatya          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/minishell.h"

int		get_binaries(t_shell *shl);
char	*get_binary_path(t_shell *shl, t_cmds *cmd);
int		remove_path(t_cmds *cmd);

/*


!!! Correction needed
	- Need to take into account the heredoc toggle while filling fd_out or the
	argument to send to print
	- Need to make some changes in this file but can't remember what
	Will add later when it comes back to me...lol
*/

int	get_binaries(t_shell *shl)
{
	t_cmds	*cmd;
	
	cmd = shl->cmds_lst;
	while (cmd)
	{
		if (cmd->args && !cmd->lvar_assignment)
			cmd->bin_path = get_binary_path(shl, cmd);
		if (!cmd->bin_path)
			exit_early(shl, NULL, "Path malloc failed");
		if (remove_path(cmd) == -1)
			exit_early(shl, NULL, "Remove path malloc failed");
		cmd = cmd->next;
	}
	return (0);
}

char	*get_binary_path(t_shell *shl, t_cmds *cmd)
{
	char	*tmp[3];
	t_lst_str	*paths;
	char	*cmd_name;

	cmd_name = *(cmd->args + cmd->skip);
	paths = shl->env_paths;
	if (!(tmp[0] = ft_strdup(cmd_name)))
			return (perror("ft_strdup-malloc failed:"), NULL);
	if ((access(tmp[0], F_OK) == 0 && tmp[0][0] == '/'))
		return (tmp[0]);
	while (paths)
	{
		if (!(tmp[1] = ft_strjoin(paths->key, "/")))
			return (free(tmp[0]), perror("tmp1-m failed:"), NULL);
		if (!(tmp[2] = ft_strjoin(tmp[1], cmd_name)))
			return (free(tmp[0]), free(tmp[1]), perror("tmp2-m failed:"), NULL);
		free(tmp[1]);
		if (access(tmp[2], F_OK) == 0)
			return (free(tmp[0]), tmp[2]);
		free(tmp[2]);
		paths = paths->next;
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
