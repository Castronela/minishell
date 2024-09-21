
#include "../include/minishell.h"

int		is_built_in(char *cmd);
void	exec_built_in(t_shell *shl);
void	exec_echo(t_cmds *cmd);

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
	if (ft_strncmp(cmd, "cd", 3) == 0)
		return (1);
	if (ft_strncmp(cmd, "pwd", 4) == 0)
		return (1);
	if (ft_strncmp(cmd, "export", 7) == 0)
		return (1);
	if (ft_strncmp(cmd, "unset", 6) == 0)
		return (1);
	if (ft_strncmp(cmd, "env", 4) == 0)
		return (1);
	if (ft_strncmp(cmd, "exit", 5) == 0)
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

void	exec_echo(t_cmds *cmd)
{
	char	*str;

	str = *cmd->args;
	if (ft_strncmp(str, "-n ", 3) == 0)
	{
		// echo with -n option
		// str = cmd->args->next->str;
		str = *(cmd->args + 1);
		ft_putstr_fd(str, cmd->fd_out);
	}
	else
	{
		// echo with no options
		ft_putstr_fd(str, cmd->fd_out);
		write(cmd->fd_out, "\n", 1);
	}
}

void	exec_cd(t_cmds *cmd)
{
	if (*(cmd->args + 1) == '.')
		return ;
	else if (ft_strncmp(*(cmd->args + 1), "..", 2) == 0)
		cwd_up();
	
}

int	path_isvalid(char *path)
{
	int			i;
	struct stat	bufr;
	
	i = stat(path, &bufr);
	
	return (i);
}