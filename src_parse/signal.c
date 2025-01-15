/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   signal.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pamatya <pamatya@student.42heilbronn.de    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/19 17:35:32 by dstinghe          #+#    #+#             */
/*   Updated: 2025/01/15 21:22:47 by pamatya          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void		set_signal(t_shell *shell, const int handler_no);

static void	signal_handler_1(int sigid, siginfo_t *info, void *ucontext);
static void	signal_handler_2(int sigid, siginfo_t *info, void *ucontext);

void	set_signal(t_shell *shell, const int handler_no)
{
	struct sigaction	sa;
	struct termios		term;

	if (handler_no == 1)
	{
		signal(SIGQUIT, SIG_IGN);
		sa.sa_sigaction = signal_handler_1;
	}
	else if (handler_no == 2)
	{
		sa.sa_sigaction = signal_handler_2;
	}
	sigemptyset(&sa.sa_mask);
	sa.sa_flags = SA_SIGINFO | SA_RESTART;
	sigaddset(&sa.sa_mask, SIGINT);
	
	if (sigaction(SIGINT, &sa, NULL) == -1)
		exit_early(shell, NULL, ERRMSG_SIGNAL);
	tcgetattr(STDIN_FILENO, &term);
	term.c_lflag &= ~ECHOCTL;
	tcsetattr(STDIN_FILENO, TCSANOW, &term);
}

static void	signal_handler_1(int sigid, siginfo_t *info, void *ucontext)
{
	(void)info;
	(void)ucontext;
	if (sigid == SIGINT)
	{
		write(STDOUT_FILENO, "\n", 1);
		rl_on_new_line();
		rl_replace_line("", 0);
		rl_redisplay();
	}
}

static void	signal_handler_2(int sigid, siginfo_t *info, void *ucontext)
{
	(void)info;
	(void)ucontext;
	if (sigid == SIGINT)
	{
		write(STDOUT_FILENO, "\n", 1);
		rl_on_new_line();
		rl_replace_line("", 0);
	}
	if (sigid == SIGQUIT)
		kill(0, SIGINT);
}
