/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   heredoc.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pamatya <pamatya@student.42heilbronn.de    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/17 15:23:48 by dstinghe          #+#    #+#             */
/*   Updated: 2024/12/20 20:25:14 by pamatya          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int			heredoc(t_shell *shell);

static int	heredoc_process_body(t_shell *shell, t_cmds *cmd_node);
static int	heredoc_prompt_parent(t_shell *shell, t_lst_str *heredoc_node,
				int (*hd_pipe)[2]);
static void	heredoc_prompt(t_shell *shell, const char *hd_delimiter,
				int fd_pipe[]);
static void	heredoc_body_var_expand(t_shell *shell, t_lst_str *heredoc_node,
				int flag_expand_vars);
static void	heredoc_read_pipe(t_shell *shell, int fd_read,
				t_lst_str *heredoc_node);

/*
Loops through every command node and checks for open heredocs
*/
int	heredoc(t_shell *shell)
{
	t_cmds	*cmd_node;

	cmd_node = shell->cmds_lst;
	while (cmd_node)
	{
		if (heredoc_process_body(shell, cmd_node))
		{
			if (write(STDOUT_FILENO, "\n", 1) < 0)
				exit_early(shell, NULL, ERRMSG_WRITE);
			return (1);
		}
		cmd_node = cmd_node->next;
	}
	return (0);
}

static int	heredoc_process_body(t_shell *shell, t_cmds *cmd_node)
{
	t_lst_str	*heredoc_node;
	int			flag_expand_vars;
	int			hd_pipe[2];

	heredoc_node = cmd_node->heredocs_lst;
	while (heredoc_node)
	{
		flag_expand_vars = count_closed_quotes(heredoc_node->key);
		remove_closed_quotes(shell, &heredoc_node->key);
		if (heredoc_prompt_parent(shell, heredoc_node, &hd_pipe))
		{
			close(hd_pipe[0]);
			return (1);
		}
		heredoc_read_pipe(shell, hd_pipe[0], heredoc_node);
		close(hd_pipe[0]);
		heredoc_body_var_expand(shell, heredoc_node, flag_expand_vars);
		heredoc_node = heredoc_node->next;
	}
	return (0);
}

static int	heredoc_prompt_parent(t_shell *shell, t_lst_str *heredoc_node,
		int (*hd_pipe)[2])
{
	int	exit_code;
	pid_t	pid;

	init_pipe_or_fork(shell, hd_pipe, &pid);
	if (pid == 0)
	{
		signal(SIGINT, SIG_DFL);
		clearout(shell);
		heredoc_prompt(shell, heredoc_node->key, *hd_pipe);
	}
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
			return (1);
	}
	return (0);
}

static void	heredoc_prompt(t_shell *shell, const char *hd_delimiter,
		int fd_pipe[])
{
	char	*input;
	int		exit_status;

	exit_status = 0;
	close(fd_pipe[0]);
	input = readline(PS2);
	while (input)
	{
		if (!ft_strncmp(hd_delimiter, input, ft_strlen(hd_delimiter) + 1))
			break ;
		if (write(fd_pipe[1], input, ft_strlen(input)) < 0 || write(fd_pipe[1],
				"\n", 1) < 0)
		{
			perror(ERRMSG_WRITE);
			exit_status = 1;
			break ;
		}
		free(input);
		input = readline(PS2);
	}
	if (input)
		free(input);
	reset_cmd_vars(shell, 1);
	close(fd_pipe[1]);
	exit(exit_status);
}

/*
Expands variables of heredoc body ONLY if 'flag_expand_vars' is 0
	- encloses entire heredoc body in double quotes
	- performs variable expansion
	- removes double quotes added earlier
*/
static void	heredoc_body_var_expand(t_shell *shell, t_lst_str *heredoc_node,
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
	new_hd_body = ft_realloc(new_hd_body, hd_body_len + 1);
	free(heredoc_node->val);
	heredoc_node->val = new_hd_body;
}

static void	heredoc_read_pipe(t_shell *shell, int fd_read,
		t_lst_str *heredoc_node)
{
	char	input[BUFFER_SIZE];
	int		bytes_read;

	bytes_read = read(fd_read, input, BUFFER_SIZE);
	if (bytes_read < 0)
		exit_early(shell, NULL, ERRMSG_READ);
	while (bytes_read)
	{
		heredoc_node->val = ft_recalloc(heredoc_node->val,
				ft_strlen2(heredoc_node->val) + bytes_read + 1);
		if (!heredoc_node->val)
			exit_early(shell, NULL, ERRMSG_MALLOC);
		ft_strlcat(heredoc_node->val, input, ft_strlen2(heredoc_node->val)
			+ bytes_read + 1);
		bytes_read = read(fd_read, input, BUFFER_SIZE);
		if (bytes_read < 0)
			exit_early(shell, NULL, ERRMSG_READ);
	}
}

