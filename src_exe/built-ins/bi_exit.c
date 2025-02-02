/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   bi_exit.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dstinghe <dstinghe@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/26 14:44:23 by pamatya           #+#    #+#             */
/*   Updated: 2025/02/02 22:05:59 by dstinghe         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/minishell.h"

void		mini_exit(t_shell *shl, t_cmds *cmd);

static int	set_retcode(t_shell *shl, t_cmds *cmd);
static int	is_valid_numstr(const char *str);

/*
Function to mimic exit command
*/
void	mini_exit(t_shell *shl, t_cmds *cmd)
{
	int	ret_code;

	// write(STDOUT_FILENO, "exit\n", 5);
	ret_code = 0;
	if (cmd->args && *(cmd->args + cmd->skip + 1))
		ret_code = set_retcode(shl, cmd);
	reset_cmd_vars(shl, 1, 1);
	clearout(shl);
	exit(ret_code);
}

static int	set_retcode(t_shell *shl, t_cmds *cmd)
{
	char	*arg;

	arg = *(cmd->args + cmd->skip + 1);
	if (!is_valid_numstr(arg))
	{
		if ((ft_fprintf_str(STDERR_FILENO, (const char *[]){ERSHL, "exit: ",
					arg, ": numeric argument required\n", NULL})) < 0)
			exit_early(shl, NULL, ERRMSG_WRITE);
		return (255);
	}
	if (count_pointers(cmd->args + cmd->skip) > 2)
	{
		if ((ft_fprintf_str(STDERR_FILENO, (const char *[]){ERSHL,
					"exit: too many arguments\n", NULL})) < 0)
			exit_early(shl, NULL, ERRMSG_WRITE);
		return (ERRCODE_GENERAL);
	}
	else
		return (ft_atoi(arg) & 0xff);
}

static int	is_valid_numstr(const char *str)
{
	size_t	index;
	int		digit_count;

	index = 0;
	while (str[index] && is_whitesp(str[index]))
		index++;
	if (str[index] == '-' || str[index] == '+')
		index++;
	digit_count = 0;
	while (str[index] && ft_isdigit(str[index]))
	{
		digit_count++;
		index++;
	}
	while (str[index] && is_whitesp(str[index]))
		index++;
	if (str[index] != 0 || !digit_count)
		return (0);
	return (1);
}
