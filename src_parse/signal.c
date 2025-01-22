/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   signal.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: david <david@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/19 17:35:32 by dstinghe          #+#    #+#             */
/*   Updated: 2025/01/22 03:43:46 by david            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void		set_signal(t_shell *shell);
void		tty_echo_sig(t_shell *shell, const bool echo);

static void	signal_handler(int sigid, siginfo_t *info, void *ucontext);

void	set_signal(t_shell *shell)
{
	struct sigaction	sa;

	signal(SIGQUIT, SIG_IGN);
	sa.sa_sigaction = signal_handler;
	sigemptyset(&sa.sa_mask);
	sa.sa_flags = SA_SIGINFO | SA_RESTART;
	sigaddset(&sa.sa_mask, SIGINT);
	if (sigaction(SIGINT, &sa, NULL) == -1)
		exit_early(shell, NULL, ERRMSG_SIGNAL);
}

void	tty_echo_sig(t_shell *shell, const bool echo)
{
	struct termios	term;

	if (!isatty(STDIN_FILENO))
		return ;
	if (tcgetattr(STDIN_FILENO, &term) < 0)
		exit_early(shell, NULL, ERRMSG_TCGETATTR);
	if (echo)
		term.c_lflag |= ECHOCTL;
	else
		term.c_lflag &= ~ECHOCTL;
	if (tcsetattr(STDIN_FILENO, TCSANOW, &term) < 0)
		exit_early(shell, NULL, ERRMSG_TCSETATTR);
}

static void	signal_handler(int sigid, siginfo_t *info, void *ucontext)
{
	(void)info;
	(void)ucontext;
	if (sigid == SIGINT)
	{
		write(STDERR_FILENO, "\n", 1);
		rl_on_new_line();
		rl_replace_line("", 0);
		rl_redisplay();
	}
}
