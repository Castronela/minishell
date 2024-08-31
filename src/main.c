
# include "minishell.h"

int	main(void)
{
    char        *input;
    // t_inputs history;
    
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
