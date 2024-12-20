#include <fcntl.h>
#include <unistd.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <sys/wait.h>
#include <errno.h>

# include "minishell.h"

int main()
{
    // int fd[2];
    int pid;
    // char buf[100];
    // char *str = "HI this is a test.";
    int stat_loc;

    // pipe(fd);
    char *str = malloc(10);
    // write(fd[1], str, strlen(str));
    // close(fd[1]);
    pid = fork();
    if (pid == 0)
    {
        // dup2(fd[0], 0);
        // close(fd[0]);
        // execve("/bin/cat", (char *[]){"cat", "To_remember.txt", NULL}, NULL);
        // perror("execve failed");
        // exit(EXIT_FAILURE);
        pause();
        // free(str);
        if (chdir("src_parse") < 0)
            exit(200);        
    }
    else if (pid > 0)
    {
        signal(SIGINT, SIG_IGN);
        printf("return code: %d\n", wait(&stat_loc));
        printf("return signal: %d\n", stat_loc); // Wait for the child process to finish
        // if stat_loc == 2, child process received SIGINT
        // if stat_loc == 256
        // close(fd[0]);
        printf("Exit status: %d\n", WEXITSTATUS(stat_loc));
        free(str);
    }
    else
    {
        perror("fork failed");
        exit(EXIT_FAILURE);
    }

    return 0;
}