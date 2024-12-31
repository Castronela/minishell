/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils_1.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pamatya <pamatya@student.42heilbronn.de    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/28 20:46:15 by castronela        #+#    #+#             */
/*   Updated: 2024/12/31 14:54:37 by pamatya          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

bool	is_quote(const char c);
void	skip_whitespaces(const char *str, size_t *index);
void	skip_quoted_str(t_shell *shell, const char *str, size_t *index);
void	init_pipe_or_fork(t_shell *shell, int (*pipe_fd)[2], pid_t *pid);
int		append_to_str(char **str, char *append, int append_len);

bool	is_quote(const char c)
{
	return (c == SQ || c == DQ);
}

void	skip_whitespaces(const char *str, size_t *index)
{
	while (str[*index] && str[*index] == SPACE)
		(*index)++;
}

/*
Modifies 'index' to point to closing quote or null terminator,
	if no closing quote found
	- stores opening quote char in 'shell->open_qt',
		to be used later for syntax check
Note: call ONLY when 'str' at 'index' is an opening quote char
*/
void	skip_quoted_str(t_shell *shell, const char *str, size_t *index)
{
	shell->open_qt = str[(*index)++];
	while (str[*index] && str[*index] != shell->open_qt)
		(*index)++;
	if (str[*index] == shell->open_qt)
		shell->open_qt = 0;
}

/*
Initializes a pipe and/or a fork if 'pipe_fd' and 'pid' are not NULL
*/
void	init_pipe_or_fork(t_shell *shell, int (*pipe_fd)[2], pid_t *pid)
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

int	append_to_str(char **str, char *append, int append_len)
{
	int	total_len;

	if (!append)
		return (0);
	if (append_len < 0)
		append_len = ft_strlen2(append);
	total_len = ft_strlen2(*str) + append_len;
	*str = ft_recalloc(*str, total_len + 1, 0);
	if (!(*str))
		return (1);
	ft_strlcat(*str, append, total_len + 1);
	return (0);
}
