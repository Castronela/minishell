/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   executions.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pamatya <pamatya@student.42heilbronn.de    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/16 00:47:46 by pamatya           #+#    #+#             */
/*   Updated: 2025/01/26 21:54:53 by pamatya          ###   ########.fr       */
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
*/
static void	exec_var_assignments(t_shell *shl, t_cmds *cmd)
{
	char		**args;
	char		*var_name;
	t_lst_str	*var_node;
	int			append;
	int			var_name_check;

	args = cmd->args;
	while (args && *args)
	{
		var_name = ft_substr(*args, 0, var_offset(*args, 0));
		if (!var_name)
			exit_early(shl, NULL, ERRMSG_MALLOC);
		var_node = ft_find_node(shl->variables, var_name, 0, 1);
		free(var_name);
		var_name_check = is_valid_name(*args, NULL);
		append = get_append_flag(var_name_check);
		// printf("var_name_check:	%d\n", var_name_check);
		// printf("append:			%d\n", append);
		if (var_node)
		{
			store_as_variable(shl, *args, append);
			add_to_environ(shl, *args, append);
		}
		else
			store_local_variable(shl, *args, append);
		args++;
	}
}

// static void	exec_var_assignments(t_shell *shl, t_cmds *cmd)
// {
// 	char		**args;
// 	char		*var_name;
// 	t_lst_str	*var_node;
// 	size_t		offset;

// 	args = cmd->args;
// 	while (args && *args)
// 	{
// 		var_name = ft_substr(*args, 0, var_offset(*args, 0));
// 		if (!var_name)
// 			exit_early(shl, NULL, ERRMSG_MALLOC);
// 		var_node = ft_find_node(shl->variables, var_name, 0, 1);
// 		offset = var_offset(*args, 1);
// 		if (var_node)
// 		{
// 			free(var_node->val);
// 			var_node->val = ft_strdup((*args + offset));
// 			if (!var_node->val)
// 				exit_early2(shl, NULL, var_name, ERRMSG_MALLOC);
// 		}
// 		else
// 			store_local_variable(shl, *args);
// 		free(var_name);
// 		args++;
// 	}
// }

/*
Function to execute built-in functions
*/
void	exec_built_in(t_shell *shl, t_cmds *cmd)
{
	char	*first_arg;
	
	if (!cmd->args || !*(cmd->args + cmd->skip))
		return ;
	update_env_var(shl, cmd, UNDERSCORE, NULL);
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
	ft_close_cmd_pipe(shl, cmd, 1 << 3 | 1 << 2);
	if (dup_std_fds(shl, cmd) < 0)
		exit_early(shl, NULL, ERRMSG_DUP2);
	if (is_built_in(cmd))
	{
		if (cmd->lvar_assignment)
			exec_var_assignments(shl, cmd);
		else
			exec_built_in(shl, cmd);
		if (cmd->next && ft_close(cmd->next->fd_in) < 0)
			exit_early(shl, NULL, NULL);
		errno = cmd->exit_code;
	}
	else
		execve(cmd->bin_path, (cmd->args + cmd->skip), shl->environ);
	reset_cmd_vars(shl, 0);
	clearout(shl);
	exit(errno);
}
