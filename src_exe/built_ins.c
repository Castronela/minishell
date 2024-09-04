
#include "../include/minishell.h"

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

void	exec_built_in(t_shell *shell)
{
	if (ft_strncmp(cmd, "echo", 5) == 0)
		exec_echo(shell);
	if (ft_strncmp(cmd, "cd", 3) == 0)
		exec_echo(shell);
	if (ft_strncmp(cmd, "pwd", 4) == 0)
		exec_echo(shell);
	if (ft_strncmp(cmd, "export", 7) == 0)
		exec_echo(shell);
	if (ft_strncmp(cmd, "unset", 6) == 0)
		exec_echo(shell);
	if (ft_strncmp(cmd, "env", 4) == 0)
		exec_echo(shell);
	if (ft_strncmp(cmd, "exit", 5) == 0)
		exec_echo(shell);
}

void	exec_echo(t_shell *shell)
{
	// redirections
	
	
	// echo with option -n
	

	// echo with no options


}