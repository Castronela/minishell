/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   bi_env.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pamatya <pamatya@student.42heilbronn.de    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/26 14:44:14 by pamatya           #+#    #+#             */
/*   Updated: 2025/01/09 02:30:31 by pamatya          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/minishell.h"

void	mini_env(t_shell *shl, t_cmds *cmd);
int		update_environ(char **var_ptr_addr, char *var_name, char *new_val);
int		count_pointers(char **dp);
char	**find_string_addr(t_shell *shl, char *str, int	n);

void	mini_env(t_shell *shl, t_cmds *cmd)
{
	int	i;

	update_env_var(shl, cmd, UNDERSCORE, NULL);
	i = -1;
	while (shl->environ[++i])
		printf("%s\n", shl->environ[i]);

	// Extras down from here
	printf("\n\n");
	t_lst_str	*vars;

	vars = shl->variables;
	while (vars)
	{
		printf("key: value ->	%s : %s\n", vars->key, vars->val);
		vars = vars->next;
	}
}
