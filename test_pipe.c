#include <fcntl.h>
#include <sys/wait.h>
#include <stdio.h>
#include <unistd.h>

int	new_close(int *fd, int *status)
{
	int ret;

	ret = 0;
	if (*fd != -1 && *fd != 0 && *fd != 1)
	{
		ret = close(*fd);
		*fd = -1;
		*status = 0;
	}
	return (0);
}

int	new_pipe(int *fd, int *status, int *fd_copy)
{
	int	ret;

	ret = pipe(fd);
	for (int j = 0; j < 2; j++)
		fd_copy[j] = fd[j];
	for (int j = 0; j < 2; j++)
		status[j] = 1;
	return (ret);
}

// int	new_print(char *process_name, int pipes, int **fds, int **copies, int **status)
int	new_print(char *process_name, int pipes, int fds[pipes][2], int copies[pipes][2], int status[pipes][2])
{
	printf("%s process:\n", process_name);
	for (int i = 0; i < pipes; i++)
	{
		for (int j = 0; j < 2; j++)
		{
			if (status[i][j] == 1)
				printf("	fd[%d][%d] =	%d	->	%d	(%d	->	open)\n", i, j, copies[i][j], fds[i][j], status[i][j]);
			else if (status[i][j] == 0)
				printf("	fds[%d][%d] =	%d	->	%d	(%d	->	closed)\n", i, j, copies[i][j], fds[i][j], status[i][j]);
		}
	}
	printf("\n");
	return (0);
}

int main(void)
{
	int	fd[3][2];
	int	fd_copy[3][2];
	int	status[3][2];
	int	x;

	x = 25;
	for (int i = 0; i < 3; i++)
	{
		if (new_pipe(fd[i], status[i], fd_copy[i]) < 0)
			return (-1);
	}

	pid_t pid1 = fork();
	if (pid1 < 0)
		return (-1);
	if (pid1 == 0)
	{
		int	a;
		new_close(&(fd[0][1]), &(status[0][1]));
		new_close(&(fd[1][0]), &(status[1][0]));
		new_close(&(fd[2][0]), &(status[2][0]));
		new_close(&(fd[2][1]), &(status[2][1]));
		if (read(fd[0][0], &a, sizeof(int)) < 0)
			return (-2);
		a += 5;
		if (write(fd[1][1], &a, sizeof(int)) <0)
			return (-2);
		new_close(&(fd[0][0]), &(status[0][0]));
		new_close(&(fd[1][1]), &(status[1][1]));

		new_print("Child 1", 3, fd, fd_copy, status);
		return (0);
	}

	pid_t pid2 = fork();
	if (pid2 < 0)
		return (-1);
	if (pid2 == 0)
	{
		int b;

		new_close(&(fd[1][1]), &(status[1][1]));
		new_close(&(fd[2][0]), &(status[2][0]));
		new_close(&(fd[0][0]), &(status[0][0]));
		new_close(&(fd[0][1]), &(status[0][1]));
		if (read(fd[1][0], &b, sizeof(int)) < 0)
			return (-3);
		b += 5;
		if (write(fd[2][1], &b, sizeof(int)) < 0)
			return (-3);
		new_close(&(fd[1][0]), &(status[1][0]));
		new_close(&(fd[2][1]), &(status[2][1]));

		new_print("Child 2", 3, fd, fd_copy, status);
		return (0);
	}

	new_close(&(fd[0][0]), &(status[0][0]));
	new_close(&(fd[2][1]), &(status[2][1]));
	new_close(&(fd[1][0]), &(status[1][0]));
	new_close(&(fd[1][1]), &(status[1][1]));
	printf("\n\nx(before) =	%d\n\n", x);
	if (write(fd[0][1], &x, sizeof(int)) < 0)
		return (-1);
	waitpid(pid1, NULL, 0);
	waitpid(pid2, NULL, 0);
	if (read(fd[2][0], &x, sizeof(int)) < 0)
		return (-1);
	printf("\n\nx(after) =	%d\n\n", x);
	new_close(&(fd[0][1]), &(status[0][1]));
	new_close(&(fd[2][0]), &(status[2][0]));

	new_print("Parent", 3, fd, fd_copy, status);
	return (0);
}