#include <fcntl.h>
#include <unistd.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <sys/wait.h>
#include <errno.h>

int main()
{
    int fd[2];
    int pid;
    char buf[100];
    char *str = "HI this is a test.";

    pipe(fd);

    write(fd[1], str, strlen(str));
    close(fd[1]);
    pid = fork();
    if (pid == 0)
    {
        dup2(fd[0], 0);
        close(fd[0]);
        execve("/bin/cat", (char *[]){"cat", NULL}, NULL);
        perror("execve failed");
        exit(EXIT_FAILURE);
    }
    else if (pid > 0)
    {
        wait(NULL); // Wait for the child process to finish
        close(fd[0]);
    }
    else
    {
        perror("fork failed");
        exit(EXIT_FAILURE);
    }

    return 0;
}