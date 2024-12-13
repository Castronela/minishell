#include "minishell.h"

// TEST FUNCTIONS

void test_print_cmdlst(t_shell *shell);
void test_free_cmds(t_shell *shell);
void test_new_tokenizer(void);


void test_print_cmdlst(t_shell *shell)
{
	t_cmds *cmd_node;

	cmd_node = shell->cmds_lst;
	for (int i = 0; cmd_node; i++)
	{
		printf("Command nr. %d\n\n", i+1);
		printf("bin path: %s", cmd_node->bin_path ? cmd_node->bin_path : "");
		printf("\nargs: ");
		for (int j = 0; cmd_node->args && cmd_node->args[j]; j++)
			printf("%s  ", cmd_node->args[j]);
		printf("\nhd_str: %s", cmd_node->hd_str ? cmd_node->hd_str : "");
		printf("\nfd_in: %d", cmd_node->fd_in);
		printf("\nfd_out: %d", cmd_node->fd_out);
		printf("\nappend: %d", cmd_node->apend);
		printf("\nfile_in: %s", cmd_node->file_in ? cmd_node->file_in : "");
		printf("\nfile_out: %s", cmd_node->file_out ? cmd_node->file_out : "");
		printf("\nctl_operator: %s\n\n", cmd_node->ctl_operator ? cmd_node->ctl_operator : "");
		
		cmd_node = cmd_node->next;
	}
}

void test_free_cmds(t_shell *shell)
{
	t_cmds *cmd_node = shell->cmds_lst;
	t_cmds *cmd_node_free;

	while (cmd_node)
	{
		if (cmd_node->bin_path)
			free (cmd_node->bin_path);
		if (cmd_node->args)
			ft_free2d(cmd_node->args);
		if (cmd_node->hd_str)
			free(cmd_node->hd_str);
		if (cmd_node->file_in)
			free(cmd_node->file_in);
		if (cmd_node->file_out)
			free(cmd_node->file_out);
		if (cmd_node->ctl_operator)
			free(cmd_node->ctl_operator);
		cmd_node_free = cmd_node;
		cmd_node = cmd_node->next;
		free(cmd_node_free);
	}
}

void test_new_tokenizer(void)
{
	t_shell shell;
	// shell.cmdline = "cmd1 >file_o1 <file_i1 <<EOF arg1             |                cmd2 >>file_o2 <file_i2 arg2";
	// shell.cmdline = "cmd1 \"  arg1  \"2n<<>file1 | cmd2>>\'arg\' >file2";
	shell.prompt = "$ ";
	get_normal_input(&shell);
}