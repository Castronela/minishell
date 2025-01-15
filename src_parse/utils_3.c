/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils_3.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: david <david@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/14 20:53:24 by dstinghe          #+#    #+#             */
/*   Updated: 2025/01/15 01:21:16 by david            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

# include "minishell.h"

void		reset_cmd_vars(t_shell *shell, const int rm_tmp);
int         open_hd_tmp_file(t_shell *shell, t_lst_str *node);

static int  get_temp_file_path(t_shell *shell, t_lst_str *cur_node, 
            char *hd_counter);
static void remove_tmp_files(t_shell *shell);

/*
Nullifies all command variables
	- if 'free_before' > 0 then frees command variables
	before nullifying them
*/
void	reset_cmd_vars(t_shell *shell, const int rm_tmp)
{
	if (rm_tmp)
		remove_tmp_files(shell);
	if (shell->tmp_file_fd != -1)
		close(shell->tmp_file_fd);
	if (shell->pid)
		free(shell->pid);
	if (shell->cmdline)
		free(shell->cmdline);
	if (shell->cmds_lst)
		lst_cmds_freelst(shell);
	shell->exit_code_prev = shell->exit_code;
	shell->pid = NULL;
	shell->cmdline = NULL;
	shell->open_qt = 0;
	shell->cmds_lst = NULL;
	shell->heredoc_file_no = 0;
	shell->tmp_file_fd = -1;
	shell->exit_code = 0;
}

static void remove_tmp_files(t_shell *shell)
{
	t_cmds *cmd;
	t_lst_str *node;

	cmd = shell->cmds_lst;
	while (cmd)
	{
		node = cmd->redirs;
		while (node)
		{
			if (!node->key)
			{
				if (node->val && !access(node->val, F_OK))
					unlink(node->val);
			}
			node = node->next;
		}
		cmd = cmd->next;
	}
}

int open_hd_tmp_file(t_shell *shell, t_lst_str *node)
{
	if (get_temp_file_path(shell, node, ft_itoa(shell->heredoc_file_no)))
		return (1);
	shell->tmp_file_fd = open(node->val, O_CREAT | O_TRUNC | O_RDWR 
		| O_APPEND, 0644);
	if (shell->tmp_file_fd < 0)
	{
		shell->exit_code = errno;
		perror(ERSHL HD_CREATE_TMP_FILE);
		return (1);
	}
	return (0);
}

static int get_temp_file_path(t_shell *shell, t_lst_str *cur_node, 
    char *hd_counter)
{
	t_lst_str *tmp_node;

	if (!hd_counter)
		exit_early(shell, NULL, ERRMSG_MALLOC);
	tmp_node = ft_find_node(shell->variables, "TMPDIR", 0, 1);
	if (tmp_node && tmp_node->val)
		cur_node->val = ft_strdup(tmp_node->val);
	else
		cur_node->val = ft_strdup(PATH_TMP_DIR);
	if (access(cur_node->val, F_OK | W_OK) < 0)
	{
		shell->exit_code = errno;
		perror(ERSHL HD_ACCESS_DIR_TMP_FILE);
		return (1);
	}
	if (append_to_str(&cur_node->val, HD_TMP_FILE_NAME, -1)
		|| append_to_str(&cur_node->val, hd_counter, -1) 
		|| append_to_str(&cur_node->val, HD_TMP_FILE_EXT, -1))
	{
		free(hd_counter);
		exit_early(shell, NULL, ERRMSG_MALLOC);
	}
	free(hd_counter);
	return(0);
}
