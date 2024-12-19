#include <fcntl.h>
#include <unistd.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <sys/wait.h>
#include <errno.h>

int main()
{
    // int fd[2];
    int pid;
    char buf[100];
    // char    *cwd;
    // char *str = "HI this is a test.";

    // pipe(fd);

    // write(1, str, strlen(str));
    // close(fd[1]);
    
    printf("cwd:		%s\n", getcwd(NULL, 0));
    printf("cwd_env:	%s\n", getenv("PWD"));

    if (chdir("test_dir/../test_dir") < 0)
    {
        perror("chdir failed");
        exit(EXIT_FAILURE);
    }
    printf("cwd:    %s\n", getcwd(NULL, 0));
    printf("cwd_env:	%s\n", getenv("PWD"));
    
	if ((pid = fork()) < 0)
    {
        perror("fork failed");
        exit(EXIT_FAILURE);
    }
    if (pid == 0)
    {
        // dup2(fd[0], 0);
        // close(fd[0]);
        execve("/bin/cat", (char *[]){"cat", "test_file.txt"}, NULL);
        perror("execve failed");
        exit(EXIT_FAILURE);
    }
    else
    {
        wait(NULL); // Wait for the child process to finish
        // close(fd[0]);
    }

    return 0;
}