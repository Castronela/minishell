/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   built_ins.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pamatya <pamatya@student.42heilbronn.de    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/04 13:38:19 by pamatya           #+#    #+#             */
/*   Updated: 2025/01/09 19:28:55 by pamatya          ###   ########.fr       */
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
	// if (cmp_cstr(cmd, "echo", 1, 0) || ft_strncmp(cmd, "cd", 2) == 0 ||
	// 	ft_strncmp(cmd, "pwd", 3) == 0 || ft_strncmp(cmd, "export", 6) == 0 ||
	// 	ft_strncmp(cmd, "unset", 5) == 0 || ft_strncmp(cmd, "env", 5) == 0 ||
	// 	ft_strncmp(cmd, "exit", 4) == 0)
	// 	return (1);
	if (compare_strings(cmd, "echo", 1) || ft_strncmp(cmd, "cd", 2) == 0 ||
		ft_strncmp(cmd, "pwd", 3) == 0 || ft_strncmp(cmd, "export", 6) == 0 ||
		ft_strncmp(cmd, "unset", 5) == 0 || ft_strncmp(cmd, "env", 5) == 0 ||
		ft_strncmp(cmd, "exit", 4) == 0)
		return (1);
	else
		return (0);
}

void	exec_built_in(t_shell *shl, t_cmds *cmd)
{
	if (set_redirections(shl, cmd) < 0)
		exit_early(shl, NULL, ERRMSG_DUP2);
	// if (cmp_cstr(*cmd->args, "echo", 1, 0))
	// 	mini_echo(cmd);
	if (compare_strings(*cmd->args, "echo", 1))
		mini_echo(cmd);
	else if (compare_strings(*cmd->args, "cd", 1))
		mini_cd(shl, cmd);
	else if (compare_strings(*cmd->args, "pwd", 1))
		mini_pwd(shl, cmd);
	else if (compare_strings(*cmd->args, "export", 1))
		mini_export(shl, cmd);
	else if (compare_strings(*cmd->args, "unset", 1))
		mini_unset(shl, cmd);
	else if (compare_strings(*cmd->args, "env", 1))
		mini_env(shl, cmd);
	else if (compare_strings(*cmd->args, "exit", 1))
		mini_exit(shl);
	ft_close_cmd_pipe(shl, cmd, 0);
	ft_close_cmd_pipe(shl, cmd, 1);
	restore_std_fds(shl);
}
