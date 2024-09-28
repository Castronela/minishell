#include "minishell.h"

// TEST FUNCTIONS

void test_variable_expansion(void)
{
	t_shell	shell;

	shell.cmdline = "echo $\'USER\' $SHELL";
	shell.tokenlst = NULL;
	if (tokenizer(&shell))
	{
		ft_lst_free(&shell.tokenlst);
		return ;
	}
	if (!variable_expansion(&shell))
		print_lst(&shell.tokenlst);
	ft_lst_free(&shell.tokenlst);
}

void test_syntax_checker(void)
{
	t_shell	shell;
	shell.cmdline = "cmd1 \"  arg1  \" 2n<<file1 | >cmd2>>\'arg\' >file2>";
	shell.tokenlst = NULL;
	if (tokenizer(&shell))
	{
		ft_lst_free(&shell.tokenlst);
		return ;
	}
	if (check_syntax(&shell))
		printf("\n");
    print_lst(&shell.tokenlst);
	ft_lst_free(&shell.tokenlst);
}

int	test_heredoc(void)
{
	t_shell	shell;

	shell.hd_status = false;
	shell.hd_delimiter = NULL;

	shell.cmdline = "cat << EOF";
	if (tokenizer(&shell))
	{
		ft_lst_free(&shell.tokenlst);;
		return (1);
	}
    print_lst(&shell.tokenlst);
	printf("is heredoc: %s\n\n", is_heredoc(&shell) ? "yes" : "no");

	shell.cmdline = "  Line1  ";
	if (tokenizer(&shell))
	{
		ft_lst_free(&shell.tokenlst);
		return (1);
	}
    print_lst(&shell.tokenlst);
	printf("is heredoc: %s\n\n", is_heredoc(&shell) ? "yes" : "no");

	shell.cmdline = "EOF";
	if (tokenizer(&shell))
	{
		ft_lst_free(&shell.tokenlst);
		return (1);
	}
    print_lst(&shell.tokenlst);
	printf("is heredoc: %s\n\n", is_heredoc(&shell) ? "yes" : "no");
	
	ft_lst_free(&shell.tokenlst);
	return (0);
}

int	test_tokenizer(void)
{
	t_shell	shell;

	shell.cmdline = "cmd1 \"  arg1  \" abcd2<<abcd <file1<>><<| cmd2 \'arg\' >file2";
	shell.tokenlst = NULL;
	if (tokenizer(&shell))
	{
		ft_lst_free(&shell.tokenlst);
		return (1);
	}
	print_lst(&shell.tokenlst);
	ft_lst_free(&shell.tokenlst);
	return (0);
}

void	print_lst(t_lst_str **head)
{
	t_lst_str	*node_current;

	node_current = *head;
	while (node_current != NULL)
	{
		printf(BG_BLUE "%s" BG_BLACK "\033[0m", node_current->str);
		if (node_current->next != NULL)
			printf("   ");
		node_current = node_current->next;
	}
	if (*head != NULL)
		printf("\n");
}
