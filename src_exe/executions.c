/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   executions.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pamatya <pamatya@student.42heilbronn.de    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/16 00:47:46 by pamatya           #+#    #+#             */
/*   Updated: 2025/01/20 17:07:08 by pamatya          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/minishell.h"

void	mini_execute(t_shell *shl);
int		exec_var_assignments(t_shell *shl, t_cmds *cmd);
void	exec_built_in(t_shell *shl, t_cmds *cmd);
void	exec_pipeline(t_shell *shl, t_cmds *cmd);

static void handle_child_exit(t_shell *shl, t_cmds *cmd);
static void exec_child(t_shell *shl, t_cmds *cmd);

void	mini_execute(t_shell *shl)
{
	t_cmds *cmd;
	
	cmd = shl->cmds_lst;
	if (!cmd->exc_index && shl->total_cmds < 2)
	{
		if (!cmd->exit_code)
		{
			dup_std_fds(shl, cmd);
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
	// else
	// 	printf("NOT A BUILT-IN COMMAND\n");
}

/*

!!! !! Still getting seg fault when path is accessed after unsetting
*/
void	exec_pipeline(t_shell *shl, t_cmds *cmd)
{
	init_cmd_pipe(shl, cmd);
	if (!cmd->exit_code)
		cmd->pid = fork();
	// pa: if (cmd->pid <0) then exit?
	if (cmd->pid == 0)
		exec_child(shl, cmd);
	ft_close_cmd_pipe(shl, cmd, 0);
	ft_close_cmd_pipe(shl, cmd, 1);
	if (cmd->next)
		exec_pipeline(shl, cmd->next);
	if (cmd->pid != -1)
		handle_child_exit(shl, cmd);	
}

static void handle_child_exit(t_shell *shl, t_cmds *cmd)
{
	int 	ec;
	
	if (waitpid(cmd->pid, &ec, 0) == -1)
		exit_early(shl, NULL, ERRMSG_WAITPID);
	if (WIFSIGNALED(ec))
		cmd->exit_code = 128 + WTERMSIG(ec);
	else if (WIFEXITED(ec))
		cmd->exit_code = WEXITSTATUS(ec);
}

static void exec_child(t_shell *shl, t_cmds *cmd)
{
	signal(SIGQUIT, SIG_DFL);
	if (cmd->next && ft_close(cmd->next->fd_in))
		exit_early(shl, NULL, NULL);	// pa: exit_early if next command exists?
	if (dup_std_fds(shl, cmd) < 0)		// pa: redir_std_fds
		exit_early(shl, NULL, ERRMSG_DUP2);
	if (!cmd->exc_index)
	{
		exec_built_in(shl, cmd);
		restore_std_fds(shl);			// Don't require this since it's in child
		reset_cmd_vars(shl, 0);
		clearout(shl);
		exit(cmd->exit_code);	// Should exit with and exit_code by checking success/failure
	}
	// test_print_1cmd(shl, cmd, 30);
	execve(cmd->bin_path, (cmd->args + cmd->skip), shl->environ);
	reset_cmd_vars(shl, 0);
	clearout(shl);
	exit(errno);
}
