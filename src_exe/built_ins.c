/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   built_ins.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pamatya <pamatya@student.42heilbronn.de    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/04 13:38:19 by pamatya           #+#    #+#             */
/*   Updated: 2024/12/10 17:56:22 by pamatya          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */


#include "../include/minishell.h"

int		is_built_in(char *cmd);
void	exe_built_in(t_shell *shl);
void	exe_echo(t_shell *shl);
void	exe_cd(t_shell *shl);


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
	if (ft_strncmp(cmd, "echo", 5) == 0)
		return (1);
	else if (ft_strncmp(cmd, "cd", 3) == 0)
		return (1);
	else if (ft_strncmp(cmd, "pwd", 4) == 0)
		return (1);
	else if (ft_strncmp(cmd, "export", 7) == 0)
		return (1);
	else if (ft_strncmp(cmd, "unset", 6) == 0)
		return (1);
	else if (ft_strncmp(cmd, "env", 4) == 0)
		return (1);
	else if (ft_strncmp(cmd, "exit", 5) == 0)
		return (1);
	return (0);
}

void	exe_built_in(t_shell *shl)
{
	if (ft_strncmp(shl->cmds_lst->bin, "echo", 5) == 0)
		exe_echo(shl->cmds_lst);
	// if (ft_strncmp(shl->cmds_lst->bin, "cd", 3) == 0)
	// 	exe_cd(shl->cmds_lst);
	// if (ft_strncmp(shl->cmds_lst->bin, "pwd", 4) == 0)
	// 	exe_pwd(shl->cmds_lst);
	// if (ft_strncmp(shl->cmds_lst->bin, "export", 7) == 0)
	// 	exe_export(shl->cmds_lst);
	// if (ft_strncmp(shl->cmds_lst->bin, "unset", 6) == 0)
	// 	exe_unset(shl->cmds_lst);
	// if (ft_strncmp(shl->cmds_lst->bin, "env", 4) == 0)
	// 	exe_env(shl->cmds_lst);
	// if (ft_strncmp(shl->cmds_lst->bin, "exit", 5) == 0)
	// 	exe_exit(shl->cmds_lst);
}
