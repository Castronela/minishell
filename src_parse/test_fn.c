#include "minishell.h"

// TEST FUNCTIONS

void test_print_cmdlst(t_shell *shell, int spacing);
void test_free_cmds(t_shell *shell);
void test_new_tokenizer(char **envp);
void test_var_exp(char **envp);


void test_print_cmdlst(t_shell *shell, int spacing)
{
	t_cmds *cmd_node;

	printf("\n%*s%s\n\n", spacing, "COMMANDLINE : ", shell->cmdline);
	cmd_node = shell->cmds_lst;
	for (int i = 0; cmd_node; i++)
	{
		printf("Command nr. %d\n\n", i+1);
		printf("%*s%s", spacing, "bin path : ", cmd_node->bin_path ? cmd_node->bin_path : "");
		printf("\n%*s", spacing, "args : ");
		for (int j = 0; cmd_node->args && cmd_node->args[j]; j++)
			printf("(%s) ", cmd_node->args[j]);
		for (t_lst_str *node = cmd_node->heredocs_lst; node; node = node->next)
		{
			printf("\n\n%*s%s", spacing, "HereDoc delimiter : ", node->key ? node->key : "");
			printf("\n%*s%s", spacing, "HereDoc body : ", node->val ? node->val : "");
		}
		printf("\n\n%*s%d", spacing, "fd_in : ", cmd_node->fd_in);
		printf("\n%*s%s", spacing, "file_in : ", cmd_node->file_in ? cmd_node->file_in : "");
		printf("\n\n%*s%d", spacing, "fd_out : ", cmd_node->fd_out);
		printf("\n%*s%s", spacing, "file_out : ", cmd_node->file_out ? cmd_node->file_out : "");
		printf("\n%*s%d", spacing, "append : ", cmd_node->apend);
		printf("\n\n%*s%s\n\n", spacing, "ctl_operator : ", cmd_node->ctl_operator ? cmd_node->ctl_operator : "");
		
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
		if (cmd_node->heredocs_lst)
			ft_lst_free(&cmd_node->heredocs_lst);
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

void test_new_tokenizer(char **envp)
{
	t_shell shell;
	init_shell(&shell, envp);
	reset_cmd_vars(&shell, 0);

	// shell.cmdline = ft_strdup("cmd1 <<file_o1 <file_i1 <<EOF arg1 |cmd2 <<file_o2 >file_i2 arg2");
	// shell.cmdline = ft_strdup("cmd1 $USER");

	shell.prompt = "$ ";
	get_normal_input(&shell);

	// parser(&shell);
	// heredoc(&shell);
	// test_print_cmdlst(&shell, 30);
	reset_cmd_vars(&shell, 1);
	clearout(&shell);
}

void test_var_exp(char **envp)
{
	t_shell shell;
	char *str = ft_strdup("$\"USER\"$USER$SHELL");
	init_shell(&shell, envp);
	t_lst_str new = { .key = ft_strdup("VAR"), .val = ft_strdup("value"), .prev = NULL, .next = NULL};
	ft_lst_addback(&shell.variables, &new);
	// for(t_lst_str *node = shell.variables; node; node = node->next)
	// 	printf("%30s : %s\n", node->key, node->val);
	// printf("%s\n", get_var_value(&shell, str));
	var_expansion(&shell, &str);
	printf("%s\n", str);
	free(str);
	// clearout(&shell);
}
