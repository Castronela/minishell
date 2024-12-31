/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   heredoc_2.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pamatya <pamatya@student.42heilbronn.de    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/26 20:24:33 by dstinghe          #+#    #+#             */
/*   Updated: 2024/12/31 14:54:17 by pamatya          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static int	heredoc_prep_prompt(t_shell *shell, int (*hd_pipe)[2]);
static void	heredoc_prompt(t_shell *shell, int fd_pipe[]);
static char	*heredoc_read_pipe(t_shell *shell, int fd_read);

/*
Retrieves all necessary Heredoc body from user
*/
int	heredoc_get_body(t_shell *shell, t_lst_str *heredoc_node)
{
	char	*input;
	int		hd_pipe[2];

	while (true)
	{
		if (heredoc_prep_prompt(shell, &hd_pipe))
			return (1);
		input = heredoc_read_pipe(shell, hd_pipe[0]);
		if (!input)
			break ;
		else if (!ft_strncmp(input, heredoc_node->key,
				ft_strlen(heredoc_node->key) + 1))
			break ;
		else if (append_to_str(&heredoc_node->val, input, -1))
		{
			free(input);
			close(hd_pipe[0]);
			exit_early(shell, NULL, ERRMSG_MALLOC);
		}
		free(input);
	}
	if (input)
		free(input);
	close(hd_pipe[0]);
	return (0);
}

/*
Function to prepare and launch heredoc prompt in a child process
*/
static int	heredoc_prep_prompt(t_shell *shell, int (*hd_pipe)[2])
{
	int		exit_code;
	pid_t	pid;

	init_pipe_or_fork(shell, hd_pipe, &pid);
	if (pid == 0)
		heredoc_prompt(shell, *hd_pipe);
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
Prompts user for Heredoc body
	- Child process, started from 'heredoc_prep_prompt'
	- frees all vars from 'shell'
	- sets signal SIGINT to default
	- prompts user for input and writes it into the pipe
*/
static void	heredoc_prompt(t_shell *shell, int fd_pipe[])
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
		if (write(fd_pipe[1], input, ft_strlen(input)) < 0 || write(fd_pipe[1],
				"\n", 1) < 0)
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
Expands variables of heredoc body ONLY if 'flag_expand_vars' is 0
	- encloses entire heredoc body in double quotes
	- performs variable expansion
	- removes double quotes added earlier
*/
void	heredoc_body_var_expand(t_shell *shell, t_lst_str *heredoc_node,
		int flag_expand_vars)
{
	char	*new_hd_body;
	size_t	hd_body_len;

	if (flag_expand_vars)
		return ;
	hd_body_len = ft_strlen2(heredoc_node->val);
	new_hd_body = ft_calloc(hd_body_len + 3, sizeof(*new_hd_body));
	if (!new_hd_body)
		exit_early(shell, NULL, ERRMSG_MALLOC);
	new_hd_body[0] = '\"';
	new_hd_body[hd_body_len] = '\"';
	ft_strlcpy2(&new_hd_body[1], heredoc_node->val, hd_body_len);
	var_expansion(shell, &new_hd_body);
	hd_body_len = ft_strlen2(new_hd_body);
	new_hd_body = ft_memmove(new_hd_body, &new_hd_body[1], hd_body_len);
	new_hd_body = ft_recalloc(new_hd_body, hd_body_len + 1, 0);
	if (!new_hd_body)
		exit_early(shell, NULL, ERRMSG_MALLOC);
	free(heredoc_node->val);
	heredoc_node->val = new_hd_body;
}

/*
Return allocated string of all content of 'fd_read'
	- if NULL is returned 'fd_read' is closed
*/
static char	*heredoc_read_pipe(t_shell *shell, int fd_read)
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
