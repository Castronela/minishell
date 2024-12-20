# include "minishell.h"

int	main(int ac, char **av, char **envp)
{
    t_shell  shl;

	if (ac != 1)
		arg_error(av);
	init_shell(&shl, envp);
	start_shell(&shl);
	clearout(&shl);
	ft_printf("Finished\n");
}
