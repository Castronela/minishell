/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils_1.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dstinghe <dstinghe@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/28 20:46:15 by castronela        #+#    #+#             */
/*   Updated: 2024/12/26 18:15:18 by dstinghe         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

bool is_quote(const char c);
void skip_whitespaces(const char *str, size_t *index);
void skip_quoted_str(t_shell *shell, const char *str, size_t *index);
bool is_redir(const char *str, const size_t index);
bool is_control(const char *str, const size_t index);
bool is_special_param(const char *str, const size_t index);
size_t find_longest_match_length(const char *str, const char *pattern[]);
void reset_cmd_vars(t_shell *shell, int free_before);
void init_pipe_or_fork(t_shell *shell, int (*pipe_fd)[2], pid_t *pid);
int	append_to_str(char **str, char *append, int append_len);


bool is_quote(const char c)
{
	return (c == SQ || c == DQ);		
}

void skip_whitespaces(const char *str, size_t *index)
{
	while(str[*index] && str[*index] == SPACE)
		(*index)++;
}

/*
Modifies 'index' to point to closing quote or null terminator, if no closing quote found 
    - stores opening quote char in 'shell->open_qt', to be used later for syntax check
Note: call ONLY when 'str' at 'index' is an opening quote char
*/
void skip_quoted_str(t_shell *shell, const char *str, size_t *index)
{
	shell->open_qt = str[(*index)++];
	while (str[*index] && str[*index] != shell->open_qt)
		(*index)++;
	if (str[*index] == shell->open_qt)
		shell->open_qt = 0;
}
/*

!!! Not sure if we can use bool or its std library. Need to check this.
*/
bool is_redir(const char *str, const size_t index)
{
	return (find_longest_match_length(&str[index], (const char *[]){REDIRECTION_OPERATORS, NULL}));
}

/*


!!! Not sure if we can use bool or its std library. Need to check this.
*/
bool is_control(const char *str, const size_t index)
{
	return (find_longest_match_length(&str[index], (const char *[]){CONTROL_OPERATORS, NULL}));
}

bool is_special_param(const char *str, const size_t index)
{
	return(find_longest_match_length(&str[index], (const char*[]){SPECIAL_OPERATORS, NULL}));
}

/*
Returns length of longest 'pattern' that matches 'str' (w/o null terminator) or 0, if no matches found
*/
size_t find_longest_match_length(const char *str, const char *pattern[])
{
	size_t index;
	size_t pattern_len;
	size_t highest_len;
	
	highest_len = 0;
	index = -1;
	while (pattern[++index])
	{
		pattern_len = ft_strlen(pattern[index]);
		if (!ft_strncmp(str, pattern[index], pattern_len))
		{
			if (pattern_len > highest_len)
				highest_len = pattern_len;
		}
	}
	return (highest_len);
}

/*
Nullifies all command variables
	- if 'free_before' > 0 then frees command variables
	before nullifying them
*/
void reset_cmd_vars(t_shell *shell, int free_before)
{
	if (free_before)
    {
        // if (shell->pipe_fds)
		// 	ft_free2dint(shell->pipe_fds);
		if (shell->pid)
			free(shell->pid);
		if (shell->cmdline)
            free(shell->cmdline);
        if (shell->cmds_lst)
            lst_cmds_freelst(shell);
    }
	// shell->pipe_fds = NULL;
	shell->pid = NULL;
    shell->cmdline = NULL;
    shell->cmds_lst = NULL;
    shell->open_qt = 0;
}

/*
Initializes a pipe and/or a fork if 'pipe_fd' and 'pid' are not NULL

!!! Need to check if this fn has been called somewhere. Potentially disruptive.
*/
void init_pipe_or_fork(t_shell *shell, int (*pipe_fd)[2], pid_t *pid)
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
