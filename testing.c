#include <fcntl.h>
#include <unistd.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <sys/wait.h>
#include <errno.h>

// # include "minishell.h"
# include "lib/includes/libft.h"

int main()
{
    char *str = malloc(6);
    strcpy(str, "Hello");
    free(str);
    printf("re: %s\n", str);
    // // int fd[2];
    // // int fd[2];
    // int pid;
    // char buf[100];
    // // char    *cwd;
    // // char *str = "HI this is a test.";
    // // char buf[100];
    // // char *str = "HI this is a test.";
    // int stat_loc;

    // // ft_pipe(fd);

    // // write(1, str, strlen(str));
    // // ft_close(fd[1]);
    
    // printf("cwd:		%s\n", getcwd(NULL, 0));
    // printf("cwd_env:	%s\n", getenv("PWD"));

    // if (chdir("test_dir/../test_dir") < 0)
    // {
    //     perror("chdir failed");
    //     exit(EXIT_FAILURE);
    // }
    // printf("cwd:    %s\n", getcwd(NULL, 0));
    // printf("cwd_env:	%s\n", getenv("PWD"));
    
	// if ((pid = ft_fork()) < 0)
    // {
    //     perror("fork failed");
    //     exit(EXIT_FAILURE);
    // }
    // // ft_pipe(fd);
    // char *str = malloc(10);
    // // write(fd[1], str, strlen(str));
    // // ft_close(fd[1]);
    // pid = ft_fork();
    // if (pid == 0)
    // {
    //     // dup2(fd[0], 0);
    //     // ft_close(fd[0]);
    //     execve("/bin/cat", (char *[]){"cat", "test_file.txt"}, NULL);
    //     perror("execve failed");
    //     exit(EXIT_FAILURE);
    //     // dup2(fd[0], 0);
    //     // ft_close(fd[0]);
    //     // execve("/bin/cat", (char *[]){"cat", "To_remember.txt", NULL}, NULL);
    //     // perror("execve failed");
    //     // exit(EXIT_FAILURE);
    //     pause();
    //     // free(str);
    //     if (chdir("src_parse") < 0)
    //         exit(200);        
    // }
    // else if (pid != 0)
    // {
    //     wait(NULL); // Wait for the child process to finish
    //     // ft_close(fd[0]);
    //     signal(SIGINT, SIG_IGN);
    //     printf("return code: %d\n", wait(&stat_loc));
    //     printf("return signal: %d\n", stat_loc); // Wait for the child process to finish
    //     // if stat_loc == 2, child process received SIGINT
    //     // if stat_loc == 256
    //     // ft_close(fd[0]);
    //     printf("Exit status: %d\n", WEXITSTATUS(stat_loc));
    //     free(str);
    // }
    // else
    // {
    //     perror("fork failed");
    //     exit(EXIT_FAILURE);
    // }

    // return 0;
}