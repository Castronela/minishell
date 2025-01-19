/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   executions.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: david <david@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/16 00:47:46 by pamatya           #+#    #+#             */
/*   Updated: 2025/01/19 03:44:10 by david            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/minishell.h"

void	mini_execute(t_shell *shl);
int		exec_var_assignments(t_shell *shl, t_cmds *cmd);
void	exec_built_in(t_shell *shl, t_cmds *cmd);
void	exec_pipeline(t_shell *shl, t_cmds *cmd);

static void handle_child_exit(t_shell *shl);
static void exec_child(t_shell *shl, t_cmds *cmd);
static void print_error_after_exit(t_shell *shl);

void	mini_execute(t_shell *shl)
{
	t_cmds *cmd;
	
	cmd = shl->cmds_lst;
	if (is_built_in(cmd) && shl->total_cmds == 1)
		exec_built_in(shl, cmd);	
	else
	{
		exec_pipeline(shl, cmd);
		print_error_after_exit(shl);
	}
	while(cmd->next)
		cmd = cmd->next;
	shl->exit_code = cmd->exit_code;
}

static void print_error_after_exit(t_shell *shl)
{
	t_cmds *cmd;
	int 	child_exited;

	child_exited = 1;
	cmd = lst_cmds_find_node(shl->cmds_lst, 0, child_exited);
	while (cmd)
	{
		if (cmd->exit_code)
		{
			if (cmd->exit_code == ENOENT && shl->env_paths && cmd->exc_index)
				ft_fprintf_str(STDERR_FILENO, (const char *[]) {ERSHL, 
					cmd->bin_path, ERRMSG_CMD_NOT_FOUND, NULL});
			else if (cmd->exc_index)
				ft_fprintf_str(STDERR_FILENO, (const char *[]) {ERSHL, 
					cmd->bin_path, ": ", strerror(cmd->exit_code), "\n", NULL});
			if (cmd->exit_code == ENOENT)
				cmd->exit_code = ERRCODE_CMD_OR_FILE_NOT_FOUND;
			else if (cmd->exit_code == EACCES)
				cmd->exit_code = ERRCODE_CMD_CNOT_EXEC;
		}
		child_exited++;
		cmd = lst_cmds_find_node(shl->cmds_lst, 0, child_exited);
	}
}

/*
Function to execute variable assignments


Note:	Here, the cmd->skip parameter should not be used as it is not executing
		a command but rather variable assignments, which the cmd->skip parameter
		is designed to skip
*/
int	exec_var_assignments(t_shell *shl, t_cmds *cmd)
{
	char		**args[2];
	t_lst_str	*var_node;
	size_t		offset;

	args[0] = cmd->args;
	while (*(args[0]))
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
	return (0);
}

/*
Function to execute built-in functions
*/
void	exec_built_in(t_shell *shl, t_cmds *cmd)
{
	char	*first_arg;
	
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
		mini_exit(shl);
	else
		printf("NOT A BUILT-IN COMMAND\n");
	ft_close_cmd_pipe(shl, cmd, 0);
	ft_close_cmd_pipe(shl, cmd, 1);
}

/*

!!! !! Still getting seg fault when path is accessed after unsetting
*/
void	exec_pipeline(t_shell *shl, t_cmds *cmd)
{
	init_cmd_pipe(shl, cmd);
	if (!cmd->exit_code)
		cmd->pid = fork();
	if (cmd->pid == 0)
		exec_child(shl, cmd);
	ft_close(cmd->fd_in);
	ft_close(cmd->fd_out);
	if (cmd->next)
		exec_pipeline(shl, cmd->next);
	if (cmd->pid != -1)
		handle_child_exit(shl);	
}

static void handle_child_exit(t_shell *shl)
{
	int 	ec;
	pid_t 	pid;
	t_cmds 	*cmd;
	
	pid = wait(&ec);
	if (pid == -1)
		exit_early(shl, NULL, ERRMSG_WAITPID);
	cmd = lst_cmds_find_node(shl->cmds_lst, pid, 0);
	if (WIFEXITED(ec))
		cmd->exit_code = WEXITSTATUS(ec);
	cmd->exit_order = ++shl->last_exited_child;
}

static void exec_child(t_shell *shl, t_cmds *cmd)
{
	if (cmd->next && ft_close(cmd->next->fd_in))
		exit_early(shl, NULL, NULL);
	if (dup_std_fds(shl, cmd) < 0)
		exit_early(shl, NULL, ERRMSG_DUP2);
	if (is_built_in(cmd))
	{
		exec_built_in(shl, cmd);
		restore_std_fds(shl);
		exit_early(shl, NULL, NULL);
	}
	// test_print_1cmd(shl, cmd, 30);
	execve(cmd->bin_path, (cmd->args + cmd->skip), shl->environ);
	ft_close(cmd->fd_in);
	ft_close(cmd->fd_out);
	exit_early(shl, NULL, NULL);
}
