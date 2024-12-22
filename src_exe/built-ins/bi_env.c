/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   bi_env.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pamatya <pamatya@student.42heilbronn.de    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/26 14:44:14 by pamatya           #+#    #+#             */
/*   Updated: 2024/12/22 20:23:46 by pamatya          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/minishell.h"

void	mini_env(t_shell *shl, t_cmds *cmd);
void	update_last_var(t_shell *shl, t_cmds *cmd);
int		update_var_str(char *var_pointer, char *var_name, char *new_val);
int		count_pointers(char **dp);
char	*find_string_ptr(char **dp, char *str, int	n);

void	mini_env(t_shell *shl, t_cmds *cmd)
{
	int	i;

	i = -1;
	update_last_var(shl, cmd);
	while (shl->env_str[++i])
		printf("%s\n", shl->env_str[i]);
}

/*
Function to update value of $_ variable in all places
  - shl->env_str
  - shl->env
  - shl->variables
*/
void	update_last_var(t_shell *shl, t_cmds *cmd)
{
	char		*_lv;
	t_lst_str	*_lvenv[2];
	t_lst_str	*_lvvar[2];

	_lv = find_string_ptr(shl->env_str, "_=", 2);
	if (update_var_str(_lv, "_=", cmd) == -1)
		exit_early(shl, NULL, ERRMSG_MALLOC);
	_lvenv[0] = ft_find_node(shl->env, "_=", 0, 0);
	_lvenv[1] = ft_lst_new(ft_strjoin("_=", cmd->bin_path), NULL);
	ft_replace_node(_lvenv[0], _lvenv[1]);
	_lvvar[0] = ft_find_node(shl->variables, "_", 1, 1);
	_lvvar[1] = ft_lst_new("_", cmd->args[count_pointers(cmd->args) - 1]);
	ft_replace_node(_lvvar[0], _lvvar[1]);
}

/*
Funcito to find the pointer in double pointer that matches the provided string
  - n is the number of characters to match while finding the pointer
*/
char	*find_string_ptr(char **dp, char *str, int	n)
{
	int	i;
	
	i = -1;
	while (dp[++i])
	{
		if (ft_strncmp(dp[i], str, n) == 0)
			return (dp[i]);
	}
	return (NULL);
}

/*
Funciton to replace the last char * in the shl->env_str list with new last-arg
*/
int	update_var_str(char *var_pointer, char *var_name, char *new_val)
{
	char	*new;
	char	*tmp;

	tmp = ft_strdup(new_val);
	if (!tmp)
		return (-1);
	new = ft_strjoin(var_name, tmp);
	if (!new)
		return (free(tmp), -1);
	free(tmp);
	free(var_pointer);
	var_pointer = new;
	return (0);
}

/*
Function to count the number of char pointers contained in a double char pointer
*/
int	count_pointers(char **dp)
{
	int	i;

	if (!dp || !*dp)
		return (0);
	i = 0;
	while (dp[i])
		i++;
	return (i); 
}
