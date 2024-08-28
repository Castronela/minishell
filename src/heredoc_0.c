#include "minishell.h"

/*
Returns 'true' if heredoc instance is open, sets hd_status to true,
	stores delimiter to hd_delimiter
Returns 'false' if NO heredoc instance or if heredoc instance 
    is closed by delimiter
*/
bool	is_heredoc(t_shell_data *shell)
{
	t_lsttoken	*node_current;

	node_current = *shell->tokenlst;
	while (shell->hd_status == false && node_current != NULL)
	{
		if (!ft_strncmp(node_current->token, "<<", 3))
		{
			shell->hd_status = true;
			rm_qt(node_current->next->token);
			shell->hd_delimiter = node_current->next->token;
			return (true);
		}
		node_current = node_current->next;
	}
	if (shell->hd_status == true)
	{
		while (node_current->next != NULL)
			node_current = node_current->next;
		if (!ft_strncmp(node_current->token, shell->hd_delimiter,
				ft_strlen(shell->hd_delimiter) + 1))
			return (false);
		return (true);
	}
	return (shell->hd_status);
}
