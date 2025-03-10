#include "minishell.h"

// TEST FUNCTIONS

void test_print_cmdlst(t_shell *shell, int spacing);
void test_print_1cmd(t_shell *shell, t_cmds *cmd_node, int spacing);
void test_free_cmds(t_shell *shell);
// void test_new_tokenizer(char **envp);
void test_new_tokenizer(t_shell *shl, char **envp);
void test_var_exp(char **envp);
void test_remove_quotes(void);

void test_print_envariables(t_shell *shell)
{
	for(t_lst_str *node = shell->variables; node; node = node->next)
		printf("%s=%s\n", node->key, node->val);	
}

void test_print_cmdlst(t_shell *shell, int spacing)
{
	t_cmds *cmd_node;

	printf("\n%*s%s\n", spacing, "COMMANDLINE : ", shell->cmdline);
	cmd_node = shell->cmds_lst;
	for (int i = 0; cmd_node; i++)
	{
		printf("Command nr. %d\n", i+1);
		printf("\n%*s%d", spacing, "cmd_index : ", cmd_node->cmd_index);
		printf("\n%*s%d", spacing, "exc_index : ", cmd_node->exc_index);
		printf("\n%*s%d", spacing, "lvar_assignment : ", cmd_node->lvar_assignment);
		printf("\n%*s%d", spacing, "skip : ", cmd_node->skip);
		printf("\n\n%*s%s", spacing, "bin path : ", cmd_node->bin_path ? cmd_node->bin_path : "");
		printf("\n%*s", spacing, "args : ");
		for (int j = 0; cmd_node->args && cmd_node->args[j]; j++)
			printf("(%s) ", cmd_node->args[j]);
		printf("\n\n%*s", spacing, "redirs : ");
		for (t_lst_str *node = cmd_node->redirs; node; node = node->next)
		{
			printf("\n\n%*s%s", spacing, "key : ", node->key ? node->key : "");
			printf("\n%*s%s", spacing, "val : ", node->val ? node->val : "NULL");
		}
		printf("\n\n%*s%d", spacing, "fd_in : ", cmd_node->fd_in);
		printf("\n\n%*s%d", spacing, "fd_out : ", cmd_node->fd_out);
		printf("\n\n%*s%s\n\n", spacing, "cmd_separator : ", cmd_node->cmd_separator ? cmd_node->cmd_separator : "NULL");
		printf("\n\n%*s%d\n\n", spacing, "cmd exit code : ", cmd_node->exit_code);
		printf("\n\n%*s%d\n\n", spacing, "total cmds : ", shell->total_cmds);
		printf("\n\n%*s%s\n\n", spacing, "cmd skip: ", cmd_node->args ? *(cmd_node->args + cmd_node->skip) : "NULL");
		printf("1st arg is :	%s\n",*cmd_node->args);
		cmd_node = cmd_node->next;
	}
}

void test_print_1cmd(t_shell *shell, t_cmds *cmd_node, int spacing)
{
	printf("\n%*s%s\n", spacing, "COMMANDLINE : ", shell->cmdline);

	printf("\n%*s%d", spacing, "cmd_index : ", cmd_node->cmd_index);
	printf("\n%*s%d", spacing, "exc_index : ", cmd_node->exc_index);
	printf("\n%*s%d", spacing, "lvar_assignment : ", cmd_node->lvar_assignment);
	printf("\n%*s%d", spacing, "skip : ", cmd_node->skip);
	printf("\n\n%*s%s", spacing, "bin path : ", cmd_node->bin_path ? cmd_node->bin_path : "");
	printf("\n%*s", spacing, "args : ");
	for (int j = 0; cmd_node->args && cmd_node->args[j]; j++)
		printf("(%s) ", cmd_node->args[j]);
	printf("\n\n%*s", spacing, "redirs : ");
	for (t_lst_str *node = cmd_node->redirs; node; node = node->next)
	{
		printf("\n\n%*s%s", spacing, "key : ", node->key ? node->key : "");
		printf("\n%*s%s", spacing, "val : ", node->val ? node->val : "NULL");
	}
	printf("\n\n%*s%d", spacing, "fd_in : ", cmd_node->fd_in);
	printf("\n\n%*s%d", spacing, "fd_out : ", cmd_node->fd_out);
	printf("\n\n%*s%s\n\n", spacing, "cmd_separator : ", cmd_node->cmd_separator ? cmd_node->cmd_separator : "");
	printf("\n");
}

// void test_free_cmds(t_shell *shell)
// {
// 	t_cmds *cmd_node = shell->cmds_lst;
// 	t_cmds *cmd_node_free;

// 	while (cmd_node)
// 	{
// 		if (cmd_node->bin_path)
// 			free (cmd_node->bin_path);
// 		if (cmd_node->args)
// 			ft_free2d(cmd_node->args);
// 		if (cmd_node->heredocs_lst)
// 			ft_lst_free(&cmd_node->heredocs_lst);
// 		if (cmd_node->file_in)
// 			free(cmd_node->file_in);
// 		if (cmd_node->file_out)
// 			free(cmd_node->file_out);
// 		if (cmd_node->cmd_separator)
// 			free(cmd_node->cmd_separator);
// 		cmd_node_free = cmd_node;
// 		cmd_node = cmd_node->next;
// 		free(cmd_node_free);
// 	}
// }
/* Dont need this anymore `*/
// // void test_new_tokenizer(char **envp)
// void test_new_tokenizer(t_shell *shl)
// {
// 	// t_shell shell;
// 	// init_shell(&shell, envp);
// 	reset_cmd_vars(shl, 0);

// 	// shell.cmdline = ft_strdup("cmd1 <<file_o1 <file_i1 <<EOF arg1 |cmd2 <<file_o2 >file_i2 arg2");
// 	// shell.cmdline = ft_strdup("cmd1 $USER");

// 	shell.prompt = "$ ";
// 	start_shell(shl);

// 	// parser(shl);
// 	// heredoc(shl);
// 	// test_print_cmdlst(shl, 30);
// 	reset_cmd_vars(shl, 1);
// 	clearout(shl);
// }

/*
TODO clear out ambiguity of test 5 and 6
in bash $ is left out, in shell $ gets printed
*/
void test_var_exp(char **envp)
{
	t_shell shell;
	char *str;
	char *good;
	init_shell(&shell, 1, NULL, envp);
	t_lst_str *new = malloc(sizeof(*new));
	new->key = ft_strdup("VAR");
	new->val = ft_strdup("there");
	new->prev = NULL;
	new->next = NULL;
	ft_lst_addback(&shell.variables, new);
	// for(t_lst_str *node = shell.variables; node; node = node->next)
	// 	printf("%30s : %s\n", node->key, node->val);

	//Test 1
	str = ft_strdup("$VAR Hello$VAR, World$VAR");
	good = ft_strdup("there Hellothere, Worldthere");
	printf("Test 1: %s\n", str);
	var_expansion(&shell, &str);
	printf("%10s\n", !strcmp(str, good) ? BG_GREEN"good:"BG_RESET : BG_RED"bad:"BG_RESET);
	printf(C_GREEN"\t%s\n"BG_RESET, good);
	printf("\t%s\n\n", str);
	free(str);
	free(good);
	//Test 2
	str = ft_strdup("$VAR$VAR, World");
	good = ft_strdup("therethere, World");
	printf("Test 2: %s\n", str);
	var_expansion(&shell, &str);
	printf("%10s\n", !strcmp(str, good) ? BG_GREEN"good:"BG_RESET : BG_RED"bad:"BG_RESET);
	printf(C_GREEN"\t%s\n"BG_RESET, good);
	printf("\t%s\n\n", str);
	free(str);
	free(good);
	//Test 3
	str = ft_strdup("Hello'$VAR', World");
	good = ft_strdup("Hello'$VAR', World");
	printf("Test 3: %s\n", str);
	var_expansion(&shell, &str);
	printf("%10s\n", !strcmp(str, good) ? BG_GREEN"good:"BG_RESET : BG_RED"bad:"BG_RESET);
	printf(C_GREEN"\t%s\n"BG_RESET, good);
	printf("\t%s\n\n", str);
	free(str);
	free(good);
	//Test 4
	str = ft_strdup("Hello\"$VAR\", World");
	good = ft_strdup("Hello\"there\", World");
	printf("Test 4: %s\n", str);
	var_expansion(&shell, &str);
	printf("%10s\n", !strcmp(str, good) ? BG_GREEN"good:"BG_RESET : BG_RED"bad:"BG_RESET);
	printf(C_GREEN"\t%s\n"BG_RESET, good);
	printf("\t%s\n\n", str);
	free(str);
	free(good);
	//Test 5
	str = ft_strdup("Hello$'VAR', World");
	good = ft_strdup("Hello$'VAR', World");
	printf("Test 5: %s\n", str);
	var_expansion(&shell, &str);
	printf("%10s\n", !strcmp(str, good) ? BG_GREEN"good:"BG_RESET : BG_RED"bad:"BG_RESET);
	printf(C_GREEN"\t%s\n"BG_RESET, good);
	printf("\t%s\n\n", str);
	free(str);
	free(good);
	//Test 6
	str = ft_strdup("Hello$\"VAR\", World");
	good = ft_strdup("Hello$\"VAR\", World");
	printf("Test 6: %s\n", str);
	var_expansion(&shell, &str);
	printf("%10s\n", !strcmp(str, good) ? BG_GREEN"good:"BG_RESET : BG_RED"bad:"BG_RESET);
	printf(C_GREEN"\t%s\n"BG_RESET, good);
	printf("\t%s\n\n", str);
	free(str);
	free(good);
	//Test 7
	str = ft_strdup("Hello $ , World$");
	good = ft_strdup("Hello $ , World$");
	printf("Test 7: %s\n", str);
	var_expansion(&shell, &str);
	printf("%10s\n", !strcmp(str, good) ? BG_GREEN"good:"BG_RESET : BG_RED"bad:"BG_RESET);
	printf(C_GREEN"\t%s\n"BG_RESET, good);
	printf("\t%s\n\n", str);
	free(str);
	free(good);
	//Test 8
	str = ft_strdup("Hello \"$\"VAR , World$");
	good = ft_strdup("Hello \"$\"VAR , World$");
	printf("Test 8: %s\n", str);
	var_expansion(&shell, &str);
	printf("%10s\n", !strcmp(str, good) ? BG_GREEN"good:"BG_RESET : BG_RED"bad:"BG_RESET);
	printf(C_GREEN"\t%s\n"BG_RESET, good);
	printf("\t%s\n\n", str);
	free(str);
	free(good);

	clearout(&shell);
}

void test_remove_quotes(void)
{
	t_shell shell;
	char *str = ft_strdup("H\'ello\' \"the\'re\" World");
	printf("quotes removed: %lu\n", count_closed_quotes(str));
	remove_closed_quotes(&shell, &str);
	printf("%s\n", str);
	free(str);
}


/*
# We need to implement "env with no options or arguments", the output here is optional
env hello world 

# We need to imlement only "cd with only a relative or absolute path"
cd 
cd ~

# Empty export isn't set on `env` but is set on `export`
export hello
env | grep hello
export | grep hello

# This shouldn't break
unset HOME
cd ~

# Local variables are options
aa==vv
echo $aa

# Open pipe waits for input
ls |

# Undefined
<<<
<<<<
> '
echo hi |    |  < >>
echo hi |  < >>   |  |
*/