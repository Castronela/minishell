# include "minishell.h"

int	main(int ac, char **av, char **envp)
{
    (void)ac;
    (void)av;
    // (void)envp;

    test_var_exp(envp);
}