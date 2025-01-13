/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   start_shell.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dstinghe <dstinghe@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/17 21:46:09 by pamatya           #+#    #+#             */
/*   Updated: 2025/01/13 16:12:06 by dstinghe         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/minishell.h"

void		start_shell(t_shell *shl);
void		mini_execute(t_shell *shl);
void		create_pids(t_shell *shl);
void		exec_external(t_shell *shl, t_cmds *cmd, int p_index);
void		index_cmds(t_shell *shl);
int			get_total_cmds(t_shell *shl, int which);
void		restore_std_fds(t_shell *shl);
int			exec_var_assignments(t_shell *shl, t_cmds *cmd);
int			is_command(t_cmds *cmd);
// static int	skip_assignments(t_cmds *cmd);

/*
Function to start shell execution

!!!	Confirm the annotation on the break condition (on the presence of break 
	statement)
*/
void	start_shell(t_shell *shl)
{	
	while (1)
	{
		set_signal(shl, 1);
		// shl->cmdline = ft_strdup("ls | grep s | grep src");
		shl->cmdline = readline(shl->prompt);
		if (!shl->cmdline)		// Is this required??
			break ;
		if (parser(shl))
		{
			add_history(shl->cmdline);
			reset_cmd_vars(shl, 1);
			continue ;
		}
		set_signal(shl, 2);
		add_history(shl->cmdline);
		index_cmds(shl);
		get_binaries(shl);
		// test_by_print(shl);
		// test_std_fds(shl);
		mini_execute(shl);
        // test_print_cmdlst(shl, 30);
		// test_printf_fds();
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
		init_cmd_pipe(shl, cmd);
		open_file_fds(shl, cmd);
		if (cmd->args && !cmd->cmd_index)
			exec_var_assignments(shl, cmd);
		else if (cmd->args && !cmd->exc_index)
			exec_built_in(shl, cmd);
		else if (cmd->args && cmd->cmd_index)
		{
			exec_external(shl, cmd, p_index);
			p_index++;
		}
		cmd = cmd->next;
	}
}

void	exec_external(t_shell *shl, t_cmds *cmd, int p_index)
{
	int	ec;

	update_env_var(shl, cmd, UNDERSCORE, NULL);
	ec = 0;
	if ((*(shl->pid + p_index) = ft_fork()) < 0)
		exit_early(shl, NULL, ERRMSG_FORK);
	if (shl->pid[p_index] != 0)
		ft_close_cmd_pipe(shl, cmd, 0);
	if (shl->pid[p_index] == 0)
	{
		ft_close_cmd_pipe(shl, cmd, 2);
		if (set_redirections(shl, cmd) < 0)
			exit_early(shl, NULL, ERRMSG_DUP2);
		ft_close_cmd_pipe(shl, cmd, 0);
		ft_close_cmd_pipe(shl, cmd, 1);
		execve(cmd->bin_path, (cmd->args + cmd->skip), shl->environ);
		ft_fprintf_str(STDERR_FILENO, (const char *[]){"minishell: ", 
			*(cmd->args + cmd->skip), ": command not found\n", NULL});
		exit_early(shl, NULL, NULL);
	}
	ft_close_cmd_pipe(shl, cmd, 0);
	ft_close_cmd_pipe(shl, cmd, 1);
	if ((waitpid(*(shl->pid + p_index), &ec, 0)) == -1)
		exit_early(shl, NULL, ERRMSG_WAITPID);
	if (WIFEXITED(ec))
		shl->exit_code = WEXITSTATUS(ec);
}

/*/
Function that indexes all commands as well as all external commands from 1 to n
  - Returns the total number of commands, internal + external
*/
void	index_cmds(t_shell *shl)
{
	t_cmds	*cmd;
	int		total;
	int		ext;

	total = 1;
	ext = 1;
	cmd = shl->cmds_lst;
	while (cmd)
	{
		if (is_command(cmd))
		{
			cmd->cmd_index = total++;
			if (cmd->args && !is_built_in(*(cmd->args + cmd->skip)))
				cmd->exc_index = ext++;
		}
		else
			cmd->lvar_assignment = 1;
		cmd = cmd->next;
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

/*
Function to restore the STDIN_FILENO and STDOUT_FILENO to point to the terminal
*/
void	restore_std_fds(t_shell *shl)
{
	if ((dup2(shl->stdio[0], STDIN_FILENO)) == -1)
		exit_early(shl, NULL, ERRMSG_DUP2);
	if ((dup2(shl->stdio[1], STDOUT_FILENO)) == -1)
		exit_early(shl, NULL, ERRMSG_DUP2);
}

// -------------------- LOCAL VARIABLE ASSIGNMENT FUNCTIONS --------------------
/*
Function to execute variable assignments

*/
int	exec_var_assignments(t_shell *shl, t_cmds *cmd)
{
	char	**args;

	args = cmd->args;
	while (*args)
	{
		store_local_variable(shl, *args);
		args++;
	}
	return (0);
}

// static int	skip_assignments(t_cmds *cmd)
// {
// 	char	**args;
// 	int		i;
// 	int		skip;
// 	int		eq;
	
// 	args = cmd->args;
// 	skip = 0;
// 	i = 0;
// 	while (!is_command(cmd))
// 	{
// 		eq = 0;
// 		if ((*(args + skip))[0] == '=')
// 			return (-1);
// 		if (!ft_isalpha((*(args + skip))[0]) && (*(args + skip))[0] != '_')
// 			return (-1);
// 		while ((*(args + skip))[++i])
// 		{
// 			if ((*(args + skip))[i] == '=')
// 			{
// 				eq = 1;
// 				break ;
// 			}
// 			if (!ft_isalnum((*(args + skip))[i]) && (*(args + skip))[i] != '_')
// 				return (-1);
// 		}
// 		if (eq == 0)
// 			return (-1);
// 		skip++;
// 	}
// 	return (skip);
// }


/*
Function that checks if any of the arguments in the cmd struct is a command
  - Returns 1 when it finds the first command, i.e. either the variable name is 
	invalid or the '=' is not present in at least one of the args
  - Returns 0 if no commands are found and all args are proper variables, i.e.
	all args are valid variable names containing an '=' sign
  - The value at the address stored by the 'skip' pointer is incremented to
	count the total number of valid variable arguments found before a command
	was encountered
  - The value at 'skip' is for the purpose of indicating how many arguments to
	skip to get to the argument where the command and its arguments start

!!! Function too long
*/
int	is_command(t_cmds *cmd)
{
	char	**args;
	int		i;
	int		eq;

	args = cmd->args;
	while (*args)
	{
		i = 0;
		eq = 0;
		if ((*args)[0] == '=')
		return (1);
		if (!ft_isalpha((*args)[0]) && (*args)[0] != '_')
			return (1);
		while ((*args)[++i])
		{
			if ((*args)[i] == '=')
			{
				eq = 1;
				break ;
			}
			if (!ft_isalnum((*args)[i]) && (*args)[i] != '_')
				return (1);
		}
		if (!eq)
			return (1);
		args++;
		(cmd->skip)++;
	}
	return (0);
}
