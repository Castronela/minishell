/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   executions.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: david <david@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/16 00:47:46 by pamatya           #+#    #+#             */
/*   Updated: 2025/01/22 05:18:02 by david            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/minishell.h"

void	mini_execute(t_shell *shl);
void	exec_built_in(t_shell *shl, t_cmds *cmd);
void	exec_pipeline(t_shell *shl, t_cmds *cmd);

static void	exec_var_assignments(t_shell *shl, t_cmds *cmd);
static void handle_child_exit(t_shell *shl, t_cmds *cmd);
static void exec_child(t_shell *shl, t_cmds *cmd);

void	mini_execute(t_shell *shl)
{
	t_cmds *cmd;
	
	cmd = shl->cmds_lst;
	if (is_built_in(cmd) && shl->total_cmds < 2)
	{
		if (!cmd->exit_code)
		{
			dup_std_fds(shl, cmd);
			if (cmd->lvar_assignment)
				exec_var_assignments(shl, cmd);
			else
				exec_built_in(shl, cmd);
			restore_std_fds(shl);
		}
	}
	else
		exec_pipeline(shl, cmd);
	while(cmd->next)
		cmd = cmd->next;
	shl->exit_code = cmd->exit_code;
}

/*
Function to execute variable assignments

Note:	Here, the cmd->skip parameter should not be used as it is not executing
		a command but rather variable assignments, which the cmd->skip parameter
		is designed to skip

!!! !!!	This function looks detached at the moment
*/
static void	exec_var_assignments(t_shell *shl, t_cmds *cmd)
{
	char		**args[2];
	t_lst_str	*var_node;
	size_t		offset;

	args[0] = cmd->args;
	while (args[0] && *(args[0]))
	{
		args[1] = ft_split(*(args[0]), '=');
		if (!(args[1]))
			exit_early(shl, NULL, ERRMSG_MALLOC);
		var_node = ft_find_node(shl->variables, *(args[1]), 0, 1);
		offset = offset_to_env_value(*(args[0]));
		if (var_node)
		{
			free(var_node->val);
			var_node->val = ft_strdup((*(args[0]) + offset));
			if (!var_node->val)
				exit_early(shl, args[1], ERRMSG_MALLOC);
		}
		else
			store_local_variable(shl, *(args[0]));
		ft_free2d(args[1]);
		(args[0])++;
	}
}

/*
Function to execute built-in functions
*/
void	exec_built_in(t_shell *shl, t_cmds *cmd)
{
	char	*first_arg;
	
	if (!cmd->args || !*(cmd->args + cmd->skip))
		return ;
	// update_env_var(shl, cmd, UNDERSCORE, NULL);
	first_arg = *(cmd->args + cmd->skip);
	if (compare_strings(first_arg, "echo", 1))
		mini_echo(cmd);
	else if (compare_strings(first_arg, "cd", 1))
		mini_cd(shl, cmd);
	else if (compare_strings(first_arg, "pwd", 1))
		mini_pwd(shl, cmd);
	else if (compare_strings(first_arg, "export", 1))
		mini_export(shl, cmd);
	else if (compare_strings(first_arg, "unset", 1))
		mini_unset(shl, cmd);
	else if (compare_strings(first_arg, "env", 1))
		mini_env(shl, cmd);
	else if (compare_strings(first_arg, "exit", 1))
		mini_exit(shl, cmd);
}

/*

!!! !! Still getting seg fault when path is accessed after unsetting
*/
void	exec_pipeline(t_shell *shl, t_cmds *cmd)
{
	init_cmd_pipe(shl, cmd);
	if (!cmd->exit_code)
	{
		cmd->pid = fork();
		if (cmd->pid < 0)
			exit_early(shl, NULL, ERRMSG_FORK);
	}
	if (cmd->pid == 0)
		exec_child(shl, cmd);
	if (ft_close(cmd->fd_in) < 0 || ft_close(cmd->fd_out) < 0)
		exit_early(shl, NULL, NULL);
	if (cmd->next)
		exec_pipeline(shl, cmd->next);
	if (cmd->pid != -1)
		handle_child_exit(shl, cmd);	
}

static void handle_child_exit(t_shell *shl, t_cmds *cmd)
{
	int 	ec;
	
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

static void exec_child(t_shell *shl, t_cmds *cmd)
{
	tty_echo_sig(shl, true);
	signal(SIGQUIT, SIG_DFL);
	signal(SIGINT, SIG_DFL);
	if (cmd->next && ft_close(cmd->next->fd_in) < 0)
		exit_early(shl, NULL, NULL);	// pa: exit_early if next command exists?
	ft_close_cmd_pipe(shl, cmd, 1 << 3 | 1 << 2);
	if (dup_std_fds(shl, cmd) < 0)		// pa: redir_std_fds
		exit_early(shl, NULL, ERRMSG_DUP2);
	if (is_built_in(cmd))
	{
		if (cmd->lvar_assignment)
			exec_var_assignments(shl, cmd);
		else
			exec_built_in(shl, cmd);
		reset_cmd_vars(shl, 0);
		clearout(shl);
		exit(cmd->exit_code);	// Should exit with and exit_code by checking success/failure
	}
	execve(cmd->bin_path, (cmd->args + cmd->skip), shl->environ);
	ft_close_cmd_pipe(shl, cmd, 2);
	reset_cmd_vars(shl, 0);
	clearout(shl);
	exit(errno);
}
