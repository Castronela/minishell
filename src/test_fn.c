#include "minishell.h"

// TEST FUNCTIONS

void test_syntax_checker(void)
{
	t_shell_data	shell;
	shell.tokenlst = tokenlst_memalloc();
	shell.cmdline = "cmd1 \"  arg1  \" <file1> | cmd2>\'arg\' >file2";

	if (shell.tokenlst == NULL)
		return ;
	if (tokenizer(&shell))
	{
		tokenlst_memfreelist(shell.tokenlst);
		free(shell.tokenlst);
		return ;
	}
	if (!check_syntax(&shell))
    	print_lst(shell.tokenlst);
	tokenlst_memfreelist(shell.tokenlst);
	free(shell.tokenlst);
}

int	test_heredoc(void)
{
	t_shell_data	shell;

	shell.hd_status = false;
	shell.hd_delimiter = NULL;
	shell.tokenlst = tokenlst_memalloc();

	shell.cmdline = "cat << EOF";
	if (shell.tokenlst == NULL)
		return (1);
	if (tokenizer(&shell))
	{
		tokenlst_memfreelist(shell.tokenlst);
		return (1);
	}
    print_lst(shell.tokenlst);
	printf("is heredoc: %s\n\n", is_heredoc(&shell) ? "yes" : "no");

	shell.cmdline = "  Line1  ";
	if (tokenizer(&shell))
	{
		tokenlst_memfreelist(shell.tokenlst);
		return (1);
	}
    print_lst(shell.tokenlst);
	printf("is heredoc: %s\n\n", is_heredoc(&shell) ? "yes" : "no");

	shell.cmdline = "EOF";
	if (tokenizer(&shell))
	{
		tokenlst_memfreelist(shell.tokenlst);
		return (1);
	}
    print_lst(shell.tokenlst);
	printf("is heredoc: %s\n\n", is_heredoc(&shell) ? "yes" : "no");
	
	tokenlst_memfreelist(shell.tokenlst);
	free(shell.tokenlst);
	return (0);
}

int	test_tokenizer(void)
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
    free(shell.tokenlst);
	return (0);
}

void	print_lst(t_lsttoken **head)
{
	t_lsttoken	*node_current;

	node_current = *head;
	while (node_current != NULL)
	{
		printf(BG_BLUE "%s" BG_BLACK "\033[0m", node_current->token);
		if (node_current->next != NULL)
			printf("   ");
		node_current = node_current->next;
	}
	if (*head != NULL)
		printf("\n");
}
