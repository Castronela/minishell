# include "minishell.h"
// #include <../include/minishell.h>

int main(int ac, char **av, char **envp)
{
   t_shell  shl;
   
   test_initiations(&shl, ac, av, envp);
}