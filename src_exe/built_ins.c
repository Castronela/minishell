/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   built_ins.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pamatya <pamatya@student.42heilbronn.de    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/04 13:38:19 by pamatya           #+#    #+#             */
/*   Updated: 2024/12/20 17:15:49 by pamatya          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */


#include "../include/minishell.h"

int		is_built_in(char *cmd);
void	exec_built_in(t_shell *shl, t_cmds *cmd);

/*
Notes:
Expansion of provided paths need to be taken care of when the path contains "." 
or ".." within in, whether at the start or somewhere middle or even end of a path.
Bash takes care of these expansions, so these paths need to be checked every
time and expanded or replaced with absolute paths. cwd_up() fn below may be used
when ".." occurs in a path.
*/

/*
Your shell must implement the following builtins:
◦ echo with option -n
◦ cd with only a relative or absolute path
◦ pwd with no options
◦ export with no options
◦ unset with no options
◦ env with no options or arguments
◦ exit with no options
*/
// Function to check whether the command is in the built-in list
int	is_built_in(char *cmd)
{
	if (ft_strncmp(cmd, "echo", 4) == 0 || ft_strncmp(cmd, "cd", 2) == 0 ||
		ft_strncmp(cmd, "pwd", 3) == 0 || ft_strncmp(cmd, "export", 6) == 0 ||
		ft_strncmp(cmd, "unset", 5) == 0 || ft_strncmp(cmd, "env", 5) == 0 ||
		ft_strncmp(cmd, "exit", 4) == 0)
		return (1);
	else
		return (0);
}

void	exec_built_in(t_shell *shl, t_cmds *cmd)
{
	size_t	binpt_offset;

	(void)shl;
	binpt_offset = ft_strlen(cmd->bin_path) - 5;
	if (ft_strncmp(cmd->bin_path + binpt_offset, "echo", 4) == 0)
		mini_echo(cmd);
	// if (ft_strncmp(shl->cmds_lst->bin, "cd", 2) == 0)
	// 	mini_cd(shl->cmds_lst);
	// if (ft_strncmp(shl->cmds_lst->bin, "pwd", 3) == 0)
	// 	mini_pwd(shl->cmds_lst);
	// if (ft_strncmp(shl->cmds_lst->bin, "export", 6) == 0)
	// 	mini_export(shl->cmds_lst);
	// if (ft_strncmp(shl->cmds_lst->bin, "unset", 5) == 0)
	// 	mini_unset(shl->cmds_lst);
	// if (ft_strncmp(shl->cmds_lst->bin, "env", 3) == 0)
	// 	mini_env(shl->cmds_lst);
	// if (ft_strncmp(shl->cmds_lst->bin, "exit", 4) == 0)
	// 	mini_exit(shl->cmds_lst);
}
