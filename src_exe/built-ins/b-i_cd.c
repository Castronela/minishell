/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   b-i_cd.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pamatya <pamatya@student.42heilbronn.de    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/26 14:42:30 by pamatya           #+#    #+#             */
/*   Updated: 2024/12/10 17:56:22 by pamatya          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/minishell.h"

/*
Built-in cd function
  - Checks first if the path provided is just ".", in which case it does nothing
  - If the path is only "..", then cwd_up() is called which updates the cwd system variable to one step above
  - If the path contains anything else, it checks whether the path is valid and then updates the cwd system variable with the current provided path including necessary expansions from "." or ".." present within the path
*/
void	exe_cd(t_shell *shl)
{
	char	**str;

	str = shl->cmds_lst->args;
	if (ft_strncmp(*(str + 1), ".\0", 2) == 0)
		return ;
	else if (ft_strncmp(*(str + 1), "..\0", 3) == 0)
		cwd_up(shl);
	else if (path_is_dir(*(str + 1)))
		update_cwd(shl);
}

/*
Function to check whether the given path is a valid path or not
  - Returns 1 if it is a valid path
  - Returns 0 if the path is invalid, or if the path results in a file
*/
int	path_is_dir(char *path)
{
	int			i;
	struct stat	bufr;
	
	i = stat(path, &bufr);
	if (i < 0)
		return (0);
	else if (i == 0)
	{
		if (S_ISREG(bufr.st_mode))
			return (0);
		else if (S_ISDIR(bufr.st_mode))
			return (1);
	}
	return (0);
}

void cwd_up(t_shell *shl)
{
	t_lst_str	*node[2];
	char		*nwd;

	node[0] = shl->env;
	node[1] = shl->env_bak;
	nwd = shl->cur_wd;

	while (node[0])
	{
		if (ft_strncmp(node[0]->str, "PWD=", 4) == 0)
		{
			
		}
		node[0] = node[0]->next;
		node[1] = node[1]->next;
	}

}
