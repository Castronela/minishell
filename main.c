# include "minishell.h"

int	main(int ac, char **av, char **envp)
{
    t_shell  shl;

	// test_new_tokenizer();

	test_initiations(&shl, ac, av, envp);
}
