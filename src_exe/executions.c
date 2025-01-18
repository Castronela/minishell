/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   executions.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pamatya <pamatya@student.42heilbronn.de    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/16 00:47:46 by pamatya           #+#    #+#             */
/*   Updated: 2025/01/17 22:49:57 by pamatya          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/minishell.h"

void		mini_execute(t_shell *shl);
int			exec_var_assignments(t_shell *shl, t_cmds *cmd);
void		exec_built_in(t_shell *shl, t_cmds *cmd);
void		exec_pipeline(t_shell *shl, t_cmds *cmd);

static void	execve_aftermath_exit(t_shell *shl, t_cmds *cmd, int err);

// /*
// Function to kick-off execution
// */
// void	mini_execute(t_shell *shl)
// {
// 	int		p_index;
// 	t_cmds	*cmd;

// 	p_index = 0;
// 	create_pids(shl);
// 	cmd = shl->cmds_lst;
// 	while (cmd)
// 	{
// 		// if (!is_built_in(*(cmd->args + cmd->skip)) && get_binary(shl, cmd))
// 		if (cmd->exc_index && get_binary(shl, cmd))
// 		{
// 			cmd = cmd->next;
// 			continue;
// 		}
// 		init_cmd_pipe(shl, cmd);
// 		if (open_file_fds(shl, cmd, cmd->redirs))
// 			break ;
// 		if (cmd->args && cmd->lvar_assignment)
// 			exec_var_assignments(shl, cmd);
// 		else if (cmd->args && !cmd->exc_index)
// 			exec_built_in(shl, cmd);
// 		else if (cmd->args && cmd->exc_index)
// 		{
// 			exec_external(shl, cmd, p_index);
// 			p_index++;
// 		}
// 		cmd = cmd->next;
// 	}
// }

void	mini_execute(t_shell *shl)
{
	int		ec;
	t_cmds *cmd;
	
	cmd = shl->cmds_lst;
	ec = 0;
	if (is_built_in(cmd) && shl->total_cmds == 1)
	{
		if (!open_file_fds(shl, cmd, cmd->redirs))
			exec_built_in(shl, cmd);	
	}
	else
		exec_pipeline(shl, cmd);
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
	if (set_redirections(shl, cmd) < 0)
		exit_early(shl, NULL, ERRMSG_DUP2);
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
	restore_std_fds(shl);
}

/*

!!! !! Still getting seg fault when path is accessed after unsetting
*/
void	exec_pipeline(t_shell *shl, t_cmds *cmd)
{
	int pipe_fd[2];
	int ec;

	if (cmd->next)
	{
		pipe(pipe_fd);
		cmd->fd_out = pipe_fd[1];
		cmd->next->fd_in = pipe_fd[0];
	}
	cmd->pid = fork();
	if (cmd->pid == 0 && !open_file_fds(shl, cmd, cmd->redirs))
	{
		ft_close_stdcpy(shl, 2);
		ft_close(pipe_fd[0]);
		if (set_redirections(shl, cmd) < 0)
			exit_early(shl, NULL, ERRMSG_DUP2);
		if (is_built_in(cmd))
		{
			exec_built_in(shl, cmd);
			exit_early(shl, NULL, NULL);
		}
		get_binary(shl, cmd);
		execve(cmd->bin_path, (cmd->args + cmd->skip), shl->environ);
		execve_aftermath_exit(shl, cmd, errno);
	}
	else
	{
		ft_close(cmd->fd_in);
		ft_close(cmd->fd_out);
		if (cmd->next)
			exec_pipeline(shl, cmd->next);
		if ((waitpid(cmd->pid, &ec, 0)) == -1)
			exit_early(shl, NULL, ERRMSG_WAITPID);
		if (WIFEXITED(ec))
			cmd->exit_code = WEXITSTATUS(ec);
	}
}

// void	exec_pipeline(t_shell *shl, t_cmds *cmd)
// {
// 	int	ec;

// 	update_env_var(shl, cmd, UNDERSCORE, NULL);
// 	ec = 0;
// 	if ((*(shl->pid + p_index) = fork()) < 0)
// 		exit_early(shl, NULL, ERRMSG_FORK);
// 	if (shl->pid[p_index] != 0)
// 		ft_close_cmd_pipe(shl, cmd, 0);
// 	if (shl->pid[p_index] == 0)
// 	{
// 		ft_close_stdcpy(shl, 2);
// 		ft_close_cmd_pipe(shl, cmd, 2);
// 		if (set_redirections(shl, cmd) < 0)
// 			exit_early(shl, NULL, ERRMSG_DUP2);
// 		ft_close_cmd_pipe(shl, cmd, 0);
// 		ft_close_cmd_pipe(shl, cmd, 1);
// 		if (is_built_in(cmd))
// 		{
// 			exec_built_in(shl, cmd);
			
// 		}
// 		execve(cmd->bin_path, (cmd->args + cmd->skip), shl->environ);
// 		execve_aftermath_exit(shl, cmd, errno);
// 	}
// 	ft_close_cmd_pipe(shl, cmd, 0);
// 	ft_close_cmd_pipe(shl, cmd, 1);
// }

// /*

// !!! !! Still getting seg fault when path is accessed after unsetting
// */
// void	exec_pipeline(t_shell *shl, t_cmds *cmd)
// {
// 	int	ec;

// 	update_env_var(shl, cmd, UNDERSCORE, NULL);
// 	ec = 0;
// 	if ((*(shl->pid + p_index) = fork()) < 0)
// 		exit_early(shl, NULL, ERRMSG_FORK);
// 	if (shl->pid[p_index] != 0)
// 		ft_close_cmd_pipe(shl, cmd, 0);
// 	if (shl->pid[p_index] == 0)
// 	{
// 		ft_close_stdcpy(shl, 2);
// 		ft_close_cmd_pipe(shl, cmd, 2);
// 		if (set_redirections(shl, cmd) < 0)
// 			exit_early(shl, NULL, ERRMSG_DUP2);
// 		ft_close_cmd_pipe(shl, cmd, 0);
// 		ft_close_cmd_pipe(shl, cmd, 1);
// 		execve(cmd->bin_path, (cmd->args + cmd->skip), shl->environ);
// 		execve_aftermath_exit(shl, cmd, errno);
// 	}
// 	ft_close_cmd_pipe(shl, cmd, 0);
// 	ft_close_cmd_pipe(shl, cmd, 1);
// 	if ((waitpid(*(shl->pid + p_index), &ec, 0)) == -1)
// 		exit_early(shl, NULL, ERRMSG_WAITPID);
// 	if (WIFEXITED(ec))
// 		shl->exit_code = WEXITSTATUS(ec);
// }

static void	execve_aftermath_exit(t_shell *shl, t_cmds *cmd, int err)
{
	// if (!is_path(*(cmd->args + cmd->skip)) && !ft_find_node(shl->variables, "PATH", 0, 1))
	// if (!(*cmd->bin_path))
	// {
	// 	ft_fprintf_str(STDERR_FILENO, (const char *[]){"minishell: ", 
	// 		*(cmd->args + cmd->skip), ": command not found\n", NULL});
	// 	exit_early(shl, NULL, NULL);
	// }
	// else
	// {
	ft_close(cmd->fd_in);
	ft_close(cmd->fd_out);
		ft_fprintf_str(STDERR_FILENO, (const char *[]){"minishell: ", 
			*(cmd->args + cmd->skip), ": ", strerror(err), "\n", NULL});
		exit_early(shl, NULL, NULL);
	// }
}
