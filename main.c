/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pamatya <pamatya@student.42heilbronn.de    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/21 17:59:37 by pamatya           #+#    #+#             */
/*   Updated: 2025/01/04 20:27:11 by pamatya          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

# include "minishell.h"

int fd_read[15] = {-1, -1,-1, -1,-1, -1,-1, -1,-1, -1,-1, -1,-1, -1, -1};
int fd_write[15] = {-1, -1,-1, -1,-1, -1,-1, -1,-1, -1,-1, -1,-1, -1, -1};

int ft_close(int fd)
{
	for(int i = 0; i < 15; i++)
	{
		if (fd_write[i] == fd && fd_write[i] != -1)
		{
			printf("closing fd: %d\n", fd);
			fd_write[i] = -1;
			break ;
		}
	}
	for(int i = 0; i < 15; i++)
	{
		if (fd_read[i] == fd && fd_read[i] != -1)
		{
			printf("closing fd: %d\n", fd);
			fd_read[i] = -1;
			break ;
		}
	}
	int ret = close(fd);
	return ret;
}
int ft_close2(int fd)
{
	for(int i = 0; i < 15; i++)
	{
		if (fd_write[i] == fd && fd_write[i] != -1)
		{
			printf("child closing fd: %d\n", fd);
			fd_write[i] = -1;
			break ;
		}
	}
	for(int i = 0; i < 15; i++)
	{
		if (fd_read[i] == fd && fd_read[i] != -1)
		{
			printf("child closing fd: %d\n", fd);
			fd_read[i] = -1;
			break ;
		}
	}
	int ret = close(fd);
	return ret;
}

int ft_pipe(int pipefd[2])
{
	int ret = pipe(pipefd);
	for(int i = 0; i < 15; i++)
	{
		if (fd_read[i] == -1)
		{
			fd_read[i] = pipefd[0];
			break ;
		}
	}
	for(int i = 0; i < 15; i++)
	{
		if (fd_write[i] == -1)
		{
			fd_write[i] = pipefd[1];
			break ;
		}
	}
	printf("opened fd: %d\n", pipefd[0]);
	printf("opened fd: %d\n", pipefd[1]);
	return ret;
}

pid_t ft_fork()
{
	// int dup_fds[15] = {-1, -1,-1, -1,-1, -1,-1, -1,-1, -1,-1, -1,-1, -1, -1};
	// for (int i = 0; i < 15; i++)
	// {
	// 	if (open_fileds[i] != -1)
	// 	{
	// 		for (int j = 0; j < 15; j++)
	// 		{
	// 			if (dup_fds[j] == -1)
	// 			{
	// 				dup_fds[j] = open_fileds[i];
	// 				break ;
	// 			}
	// 		}
	// 	}
	// }
	// for (int i = 0; i < 15; i++)
	// {
	// 	if (dup_fds[i] != -1)
	// 	{
	// 		for (int j = 0; j < 15; j++)
	// 		{
	// 			if (open_fileds[j] == -1)
	// 			{
	// 				open_fileds[j] = dup_fds[i];
	// 				break ;
	// 			}
	// 		}
	// 	}
	// }
	// printf("duped fds\n");
	pid_t ret = fork();
	return ret;
}

void test_std_fds(t_shell *shl)
{
	printf("\nPreserved fds: ");
	printf("%d, ", shl->stdio[0]);
	printf("%d, ", shl->stdio[1]);
	printf("\n");
}

void test_printf_fds(void)
{
	printf("\nRead fds: ");
	for (int i = 0; i < 15; i++)
	{
		// if (fd_read[i] != -1)
			printf("%d, ", fd_read[i]);
	}
	printf("\nWrite fds: ");
	for (int i = 0; i < 15; i++)
	{
		// if (fd_write[i] != -1)
			printf("%d, ", fd_write[i]);
	}
	printf("\n");
}

int	main(int ac, char **av, char **envp)
{
    t_shell  shl;

	if (ac != 1)
		arg_error(av);
	init_shell(&shl, envp);
	start_shell(&shl);
	clearout(&shl);
	ft_printf("Finished\n");
	return (shl.exit_code);
}


