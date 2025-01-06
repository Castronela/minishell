/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   secondary_prompt.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dstinghe <dstinghe@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/04 14:15:41 by dstinghe          #+#    #+#             */
/*   Updated: 2025/01/06 18:02:37 by dstinghe         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int			secondary_prompt(t_shell *shell, const bool prepend_nl);
int			prep_prompt(t_shell *shell, int (*hd_pipe)[2],
				const bool append_nl);
char		*prompt_read(t_shell *shell, int fd_read);

static void	prompt_child(t_shell *shell, int fd_pipe[], const bool append_nl);
static void	init_pipe_or_fork(t_shell *shell, int (*pipe_fd)[2], pid_t *pid);

/*
Function to prompt user for additional input, using prompt string 'PS2';
To prepend a newline to cmdline, set 'prepend_nl' to non-zero number;
Return codes:
0 - normal exit
1 - returned input is NULL (received EOF)
2 - child process stopped by signal
*/
int	secondary_prompt(t_shell *shell, const bool prepend_nl)
{
	char	*input;
	int		fds[2];

	if (prepend_nl)
		append_to_str(&shell->cmdline, "\n", -1);
	else
		append_to_str(&shell->cmdline, " ", -1);
	if (!shell->cmdline)
		exit_early(shell, NULL, ERRMSG_MALLOC);
	if (prep_prompt(shell, &fds, 0))
		return (2);
	input = prompt_read(shell, fds[0]);
	close(fds[0]);
	if (!input)
		return (1);
	append_to_str(&shell->cmdline, input, -1);
	if (input)
		free(input);
	if (!shell->cmdline)
		exit_early(shell, NULL, ERRMSG_MALLOC);
	return (0);
}

/*
Function to prepare and launch secondary prompt in a child process;
To append a newline to input, set 'append_nl' to non-zero number;
*/
int	prep_prompt(t_shell *shell, int (*hd_pipe)[2], const bool append_nl)
{
	int		exit_code;
	pid_t	pid;

	init_pipe_or_fork(shell, hd_pipe, &pid);
	if (pid == 0)
		prompt_child(shell, *hd_pipe, append_nl);
	else
	{
		signal(SIGINT, SIG_IGN);
		wait4(pid, &exit_code, 0, NULL);
		close((*hd_pipe)[1]);
		if (WEXITSTATUS(exit_code))
		{
			close((*hd_pipe)[0]);
			exit_early(shell, NULL, NULL);
		}
		if (WIFSIGNALED(exit_code))
		{
			close((*hd_pipe)[0]);
			return (1);
		}
	}
	return (0);
}

/*
Function for child process; prompts user for secondary input;
To append a newline to input, set 'append_nl' to non-zero number;
*/
static void	prompt_child(t_shell *shell, int fd_pipe[], const bool append_nl)
{
	char	*input;
	int		exit_status;

	clearout(shell);
	reset_cmd_vars(shell, 1);
	signal(SIGINT, SIG_DFL);
	exit_status = 0;
	close(fd_pipe[0]);
	input = readline(PS2);
	if (input)
	{
		if (write(fd_pipe[1], input, ft_strlen(input)) < 0 || (append_nl
				&& write(fd_pipe[1], "\n", 1) < 0))
		{
			perror(ERRMSG_WRITE);
			exit_status = 1;
		}
		free(input);
	}
	close(fd_pipe[1]);
	exit(exit_status);
}

/*
Function to read and return allocated string from fd 'fd_read'
*/
char	*prompt_read(t_shell *shell, int fd_read)
{
	char	*final_str;
	char	input[BUFFER_SIZE + 1];
	int		bytes_read;

	final_str = NULL;
	while (true)
	{
		bytes_read = read(fd_read, input, BUFFER_SIZE);
		if (bytes_read <= 0)
			break ;
		input[bytes_read] = 0;
		if (append_to_str(&final_str, input, -1))
			break ;
	}
	close(fd_read);
	if (bytes_read < 0)
		exit_early(shell, NULL, ERRMSG_READ);
	if (!final_str && bytes_read)
		exit_early(shell, NULL, ERRMSG_MALLOC);
	return (final_str);
}

/*
Initializes a pipe and/or a fork if 'pipe_fd' and 'pid' are not NULL
*/
static void	init_pipe_or_fork(t_shell *shell, int (*pipe_fd)[2], pid_t *pid)
{
	if (pipe_fd)
	{
		if (pipe(*pipe_fd) < 0)
			exit_early(shell, NULL, ERRMSG_PIPE);
	}
	if (pid)
	{
		*pid = fork();
		if (*pid < 0)
		{
			if (pipe_fd)
			{
				close((*pipe_fd)[0]);
				close((*pipe_fd)[1]);
			}
			exit_early(shell, NULL, ERRMSG_FORK);
		}
	}
}
