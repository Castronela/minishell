/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   bi_export.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pamatya <pamatya@student.42heilbronn.de    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/26 14:44:28 by pamatya           #+#    #+#             */
/*   Updated: 2025/02/02 14:39:08 by pamatya          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/minishell.h"

int					mini_export(t_shell *shl, t_cmds *cmd);

static void			print_quoted_env(t_shell *shl);
static int			check_and_export_arg(t_shell *shl, char *arg, int *chex);
static int			is_valid_val(char *arg, int *i);
static t_lst_str	*get_next_var_alphabetically(t_shell *shl, t_lst_str *prev);

/*
Function for built-in export function
  - Adds the exported variable to the shl->env
  - Also adds the variable as key-val pair to the variables list

Note:	Here, the upd_str is only freed if chex[2] == 1 which indicates that
		it is a malloc'd string, otherwise it is simply a borrowed pointer
*/
int	mini_export(t_shell *shl, t_cmds *cmd)
{
	char	**arguments;
	int		chex[3];
	int		ret;
	char	*upd_str;

	upd_str = NULL;
	ret = 0;
	arguments = cmd->args + cmd->skip;
	if (*(arguments + 1) == NULL)
		return (print_quoted_env(shl), 0);
	while (*(++arguments))
	{
		upd_str = *arguments;
		if (check_and_export_arg(shl, *arguments, chex))
			cmd->exit_code = ERRCODE_GENERAL;
		if (*(arguments + 1) == NULL && chex[2] == 1)
			upd_str = get_var_component(shl, *arguments, 0);
		if (chex[0] < 1)
			ret = 1;
	}
	update_env_var(shl, cmd, UNDERSCORE, upd_str);
	if (upd_str && chex[2] == 1)
		free(upd_str);
	return (ret);
}

/*
Function to print shl->environ with quotes around each value
*/
static void	print_quoted_env(t_shell *shl)
{
	t_lst_str	*vr;

	vr = NULL;
	while (1)
	{
		vr = get_next_var_alphabetically(shl, vr);
		if (!vr)
			break ;
		if (!compare_strings(vr->key, "_", 1) && !vr->val)
		{
			if (ft_fprintf_str(STDOUT_FILENO, (const char *[]){"declare -x ",
					vr->key, "\n", NULL}) == 1)
				exit_early(shl, NULL, ERRMSG_WRITE);
		}
		else if (!compare_strings(vr->key, "_", 1))
		{
			if (ft_fprintf_str(STDOUT_FILENO, (const char *[]){"declare -x ",
					vr->key, "=\"", vr->val, "\"\n", NULL}) == 1)
				exit_early(shl, NULL, ERRMSG_WRITE);
		}
	}
}

static t_lst_str	*get_next_var_alphabetically(t_shell *shl, t_lst_str *prev)
{
	t_lst_str	*node;
	t_lst_str	*next;

	next = NULL;
	node = shl->variables;
	while (node)
	{
		if (node->key
			&& (!prev || ft_strncmp(node->key, prev->key,
					ft_strlen(prev->key) + 1) > 0) && (!next
				|| ft_strncmp(node->key, next->key,
					ft_strlen(node->key) + 1) < 0))
		{
			next = node;
		}
		node = node->next;
	}
	return (next);
}

/*
Static sub-function for export-chex and execution if chex pass
*/
static int	check_and_export_arg(t_shell *shl, char *arg, int *chex)
{
	int	i;
	int	append;

	i = 0;
	chex[0] = is_valid_name(arg, &i);
	chex[1] = is_valid_val(arg, &i);
	chex[2] = 0;
	if (chex[0] < 0)
	{
		if ((ft_fprintf_str(STDERR_FILENO, (const char *[]){ERSHL, "export: `",
					arg, ERRMSG_NO_VALID_IDENT, NULL})) < 0)
			exit_early(shl, NULL, ERRMSG_WRITE);
		return (1);
	}
	if (chex[0] >= 0 && chex[0] != 2 && chex[1] >= 0)
	{
		append = get_append_flag(chex[0]);
		add_to_environ(shl, arg, append);
		store_as_variable(shl, arg, append);
		chex[2] = 1;
	}
	if (chex[0] == 2)
		chex[2] = 1;
	return (0);
}

/*
Function to check whether the variable value is a valid one
  - chex if each character in the provided string is not a reserved bash
	character (is_bash_reserved), and that it is printable (ft_isprint)
  - Returns 0 if the variable value is found to be a reserved bash character
  - Returns -n when nth character in the argument is found to be non-printable
  - Returns 1 if the variable value is found to be valid
  - Used fns: is_bash_reserved(), ft_isprint()
*/
static int	is_valid_val(char *arg, int *i)
{
	if (arg[*i] == '\0')
		return (0);
	while (arg[*i])
	{
		if (!ft_isprint(arg[*i]))
			return (-1 * (*i));
		(*i)++;
	}
	return (1);
}
