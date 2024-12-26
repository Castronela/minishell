/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   bi_env.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pamatya <pamatya@student.42heilbronn.de    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/26 14:44:14 by pamatya           #+#    #+#             */
/*   Updated: 2024/12/26 20:03:58 by pamatya          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/minishell.h"

void	mini_env(t_shell *shl, t_cmds *cmd);
void	update_env_var(t_shell *shl, t_cmds *cmd);
int		update_var_str(char *var_pointer, char *var_name, char *new_val);
int		count_pointers(char **dp);
char	*find_string_ptr(char **dp, char *str, int	n);

void	mini_env(t_shell *shl, t_cmds *cmd)
{
	int	i;

	i = -1;
	update_env_var(shl, cmd);
	(void)cmd;
	while (shl->environ[++i])
		printf("%s\n", shl->environ[i]);
}
