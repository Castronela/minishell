#include "minishell.h"

int	main(void)
{
	tokenizer_tester();
}

// TEST FUNCTIONS

int	tokenizer_tester(void)
{
	t_shell_data	shell;

	shell.cmdline = "cmd1 \"  arg1  \" <file1 <>><<| cmd2\'arg\' >file2";
	shell.tokenlst = tokenlst_memalloc();
	if (shell.tokenlst == NULL)
		return (1);
	if (tokenizer(&shell))
	{
		tokenlst_memfreelist(shell.tokenlst);
		return (1);
	}
	print_lst(shell.tokenlst);
	tokenlst_memfreelist(shell.tokenlst);
	return (0);
}

void	print_lst(t_lsttoken **head)
{
	t_lsttoken	*node_current;

	node_current = *head;
	while (node_current != NULL)
	{
		printf(BG_BLUE "%s" BG_BLACK, node_current->token);
		if (node_current->next != NULL)
			printf("   ");
		node_current = node_current->next;
	}
	if (*head != NULL)
		printf("\n");
}
