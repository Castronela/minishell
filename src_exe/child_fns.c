/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   child_fns.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pamatya <pamatya@student.42heilbronn.de    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/02 14:24:34 by pamatya           #+#    #+#             */
/*   Updated: 2025/02/02 14:32:51 by pamatya          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/minishell.h"

void	handle_child_exit(t_shell *shl, t_cmds *cmd);
void	exec_child(t_shell *shl, t_cmds *cmd);

void	exec_child(t_shell *shl, t_cmds *cmd)
{
	tty_echo_sig(shl, true);
	signal(SIGQUIT, SIG_DFL);
	signal(SIGINT, SIG_DFL);
	if (cmd->next && ft_close(cmd->next->fd_in) < 0)
		exit_early(shl, NULL, NULL);
	ft_close_cmd_pipe(shl, cmd, 1 << 3 | 1 << 2);
	if (dup_std_fds(shl, cmd) < 0)
		exit_early(shl, NULL, ERRMSG_DUP2);
	if (is_built_in(cmd))
	{
		if (cmd->lvar_assignment)
			exec_var_assignments(shl, cmd);
		else
			exec_built_in(shl, cmd);
		errno = cmd->exit_code;
	}
	else
		execve(cmd->bin_path, (cmd->args + cmd->skip), shl->environ);
	reset_cmd_vars(shl, 0, 1);
	clearout(shl);
	exit(errno);
}

void	handle_child_exit(t_shell *shl, t_cmds *cmd)
{
	int	ec;

	signal(SIGINT, SIG_IGN);
	if (waitpid(cmd->pid, &ec, 0) == -1)
		exit_early(shl, NULL, ERRMSG_WAITPID);
	if (WIFSIGNALED(ec))
	{
		if (WTERMSIG(ec) == SIGINT)
			write(STDERR_FILENO, "\n", 1);
		else if (WTERMSIG(ec) == SIGQUIT)
			write(STDERR_FILENO, "Quit\n", 5);
		cmd->exit_code = 128 + WTERMSIG(ec);
	}
	else if (WIFEXITED(ec))
		cmd->exit_code = WEXITSTATUS(ec);
}
