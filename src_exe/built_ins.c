
#include "../include/minishell.h"

int		is_built_in(char *cmd);
void	exec_built_in(t_shell *shl);
void	exec_echo(t_shell *shl);
void	exec_cd(t_shell *shl);

/*
Your shell must implement the following builtins:
◦ echo with option -n
◦ cd with only a relative or absolute path
◦ pwd with no options
◦ export with no options
◦ unset with no options
◦ env with no options or arguments
◦ exit with no options
*/

// Function to check whether the command is in the built-in list
int	is_built_in(char *cmd)
{
	if (ft_strncmp(cmd, "echo", 5) == 0)
		return (1);
	else if (ft_strncmp(cmd, "cd", 3) == 0)
		return (1);
	else if (ft_strncmp(cmd, "pwd", 4) == 0)
		return (1);
	else if (ft_strncmp(cmd, "export", 7) == 0)
		return (1);
	else if (ft_strncmp(cmd, "unset", 6) == 0)
		return (1);
	else if (ft_strncmp(cmd, "env", 4) == 0)
		return (1);
	else if (ft_strncmp(cmd, "exit", 5) == 0)
		return (1);
	return (0);
}

void	exec_built_in(t_shell *shl)
{
	if (ft_strncmp(shl->cmds->cmds_lst->bin, "echo", 5) == 0)
		exec_echo(shl->cmds->cmds_lst);
	// if (ft_strncmp(shl->cmds->cmds_lst->bin, "cd", 3) == 0)
	// 	exec_cd(shl->cmds->cmds_lst);
	// if (ft_strncmp(shl->cmds->cmds_lst->bin, "pwd", 4) == 0)
	// 	exec_pwd(shl->cmds->cmds_lst);
	// if (ft_strncmp(shl->cmds->cmds_lst->bin, "export", 7) == 0)
	// 	exec_export(shl->cmds->cmds_lst);
	// if (ft_strncmp(shl->cmds->cmds_lst->bin, "unset", 6) == 0)
	// 	exec_unset(shl->cmds->cmds_lst);
	// if (ft_strncmp(shl->cmds->cmds_lst->bin, "env", 4) == 0)
	// 	exec_env(shl->cmds->cmds_lst);
	// if (ft_strncmp(shl->cmds->cmds_lst->bin, "exit", 5) == 0)
	// 	exec_exit(shl->cmds->cmds_lst);
}

/*
Built-in echo function
  - Need to check if this is complete/sufficient...
  - if the command contains "-n ", then it writes to fd_out without a newline
  - if command contains no -n option, then it writes to fd_out followed by a newline
*/
void	exec_echo(t_shell *shl)
{
	char	**str;

	str = shl->cmds->cmds_lst->args;
	if (ft_strncmp(*str, "-n ", 3) == 0)
	{
		// echo with -n option
		ft_putstr_fd(*str + 1, shl->cmds->cmds_lst->fd_out);
	}
	else
	{
		// echo with no options
		ft_putstr_fd(*str, shl->cmds->cmds_lst->fd_out);
		write(shl->cmds->cmds_lst->fd_out, "\n", 1);
	}
}

/*
Built-in cd function
  - Checks first if the path provided is just ".", in which case it does nothing
  - If the path is only "..", then cwd_up() is called which updates the cwd system variable to one step above
  - If the path contains anything else, it checks whether the path is valid and then updates the cwd system variable with the current provided path including necessary expansions from "." or ".." present within the path
*/
void	exec_cd(t_shell *shl)
{
	char	**str;

	str = shl->cmds->cmds_lst->args;
	if (ft_strncmp(*(str + 1), ".\0", 2) == 0)
		return ;
	else if (ft_strncmp(*(str + 1), "..\0", 3) == 0)
		cwd_up();
	else if (path_is_dir(*(str + 1)))
		update_cwd((cmd->args + 1));
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

void cwd_up(void)
{

}