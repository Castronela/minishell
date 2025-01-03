/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   start_shell.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pamatya <pamatya@student.42heilbronn.de    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/17 21:46:09 by pamatya           #+#    #+#             */
/*   Updated: 2025/01/03 21:05:22 by pamatya          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/minishell.h"

void		start_shell(t_shell *shl);
void		mini_execute(t_shell *shl);
void		create_pids(t_shell *shl);
void		exec_external(t_shell *shl, t_cmds *cmd, int p_index);
void		index_cmds(t_shell *shl);
int			get_total_cmds(t_shell *shl, int which);
static void	set_prev_exitcode(t_shell *shell);
void		restore_stdfds(t_shell *shl);

// static void	print_env(t_shell *shl);
// static void	print_shlvl(t_shell *shl);
// static void	print_cwd(t_shell *shl);
// static void	print_env_paths(t_shell *shl);

void	start_shell(t_shell *shl)
{
	while (1)
	{
		set_signal(shl);
		set_prev_exitcode(shl);
		shl->cmdline = readline(shl->prompt);
		if (!shl->cmdline)
			break ;
		// if (!(ft_strncmp(shl->cmdline, "exit", 4)))
		// 	break ;
		add_history(shl->cmdline);
		if (parser(shl) || heredoc(shl))
		{
			reset_cmd_vars(shl, 1);
			continue ;
		}
		index_cmds(shl);
		init_pipes(shl);
		get_binaries(shl);
		// test_by_print(shl);
        test_print_cmdlst(shl, 30);
		mini_execute(shl);
		reset_cmd_vars(shl, 1);
	}
}

/*
Function to kick-off execution
*/
void	mini_execute(t_shell *shl)
{
	int		p_index;
	t_cmds	*cmd;

	p_index = 0;
	create_pids(shl);
	cmd = shl->cmds_lst;
	while (cmd)
	{
		if (open_file_fds(cmd) < 0)
			exit_early(shl, NULL, ERRMSG_OPEN);
		if (cmd->exc_index == 0)
		{
			// if (set_redirections(shl, cmd) < 0)
			// 	exit_early(shl, NULL, ERRMSG_DUP2);
			update_env_var(shl, cmd, UNDERSCORE, NULL);
			exec_built_in(shl, cmd);
			// close_fds(cmd);
		}
		else
		{
			update_env_var(shl, cmd, UNDERSCORE, NULL);
			exec_external(shl, cmd, p_index);	
			p_index++;
			// close_fds(cmd);
		}
		close_fds(cmd);
		cmd = cmd->next;
	}
}

void	exec_external(t_shell *shl, t_cmds *cmd, int p_index)
{
	int	ec;

	ec = 0;
	if ((*(shl->pid + p_index) = fork()) < 0)
		exit_early(shl, NULL, ERRMSG_FORK);
	if (shl->pid[p_index] == 0)
	{
		if (cmd->close_fd[0] != -1)
		{
			if (close(cmd->close_fd[0]))
				perror("1");
		}
		if (cmd->close_fd[1] != -1)
		{
			if (close(cmd->close_fd[1]))
				perror("2");
		}
		if (set_redirections(shl, cmd) < 0)
			exit_early(shl, NULL, ERRMSG_DUP2);
		close_fds(cmd);
		execve(cmd->bin_path, cmd->args, shl->environ);
		exit_early(shl, NULL, ERRMSG_EXECVE);
	}
	if (close_fds(cmd) == -1)
		perror("-555");
	if ((waitpid(*(shl->pid + p_index), &ec, 0)) == -1)
		exit_early(shl, NULL, ERRMSG_WAITPID);
	if (WIFEXITED(ec))
		shl->exit_code = WEXITSTATUS(ec);
	// close_fds(cmd);
}

/*/
Function that indexes all commands as well as all external commands from 1 to n
  - Returns the total number of commands, internal + external
*/
void	index_cmds(t_shell *shl)
{
	t_cmds	*cmds;
	int		total;
	int		ext;

	total = 1;
	ext = 1;
	cmds = shl->cmds_lst;
	while (cmds)
	{
		cmds->cmd_index = total++;
		if (!is_built_in(*(cmds->args)))
			cmds->exc_index = ext++;
		cmds = cmds->next;
	}
}

/*
Function to get the total number of commands
  - Does so by fetching the cmd_index or exc_index of the last command in the list
  - Does not recount the indices
  - If 'which' is 0, returns total number of (internal + external) commands
  - If 'which' is 1, return total number of external commands only
  - Returns -1 for any other value of 'which'
*/
int	get_total_cmds(t_shell *shl, int which)
{
	int		total;
	t_cmds	*cmd;

	total = 0;
	cmd = shl->cmds_lst;
	while (cmd)
	{
		if (which == 0)
				total = cmd->cmd_index;
		else if (which == 1)
		{
			if (total < cmd->exc_index)
				total = cmd->exc_index;
		}
		else
			return (-1);
		cmd = cmd->next;
	}
	return (total);
}

/*
Function to malloc a pid_t pointer for the required number of pids
*/
void	create_pids(t_shell *shl)
{
	int	total_ext;
	
	total_ext = get_total_cmds(shl, 1);
	shl->pid = malloc(total_ext * sizeof(pid_t));
	if (!shl->pid)
		exit_early(shl, NULL, "PID malloc failed");
}

static void set_prev_exitcode(t_shell *shell)
{
	shell->exit_code_prev = shell->exit_code;
	shell->exit_code = 0;	
}

/*
Function to restore the STDIN_FILENO and STDOUT_FILENO to point to the terminal
*/
void	restore_stdfds(t_shell *shl)
{
	if ((dup2(STDIN_FILENO, shl->stdio[0])) == -1)
		exit_early(shl, NULL, ERRMSG_DUP2);
	if ((dup2(STDOUT_FILENO, shl->stdio[1])) == -1)
		exit_early(shl, NULL, ERRMSG_DUP2);
}
