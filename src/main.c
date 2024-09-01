
# include "minishell.h"

int	main(int ac, char **av, char **envp)
{
    char        *input;
    // t_inputs history;
    
    if (ac != 1)
    {
        ft_putstr_fd("Error: Minishell does not take any arguments\n", 2);
        return 1;
    }
    (void)av;
    
    ft_putstr_fd("Welcome to Minishell!\n", 1);
    
    while (!(ft_strncmp(input, "exit", 4)))
    {
        input = readline("Mini-Schale$");
        if (!input)
            continue;
        add_history(input); 
    }
    
    return 0;
}
