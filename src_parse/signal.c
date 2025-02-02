/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   signal.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dstinghe <dstinghe@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/19 17:35:32 by dstinghe          #+#    #+#             */
/*   Updated: 2025/02/02 22:14:41 by dstinghe         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void		set_signal(t_shell *shell);
void		tty_echo_sig(t_shell *shell, const bool echo);

static void	signal_handler(int sigid);

void	set_signal(t_shell *shell)
{
	struct sigaction	sa;

	signal(SIGQUIT, SIG_IGN);
	sa.sa_handler = signal_handler;
	sigemptyset(&sa.sa_mask);
	sa.sa_flags = SA_RESTART;
	sigaddset(&sa.sa_mask, SIGINT);
	if (sigaction(SIGINT, &sa, NULL) == -1)
		exit_early(shell, NULL, ERRMSG_SIGNAL);
}

static void	signal_handler(int sigid)
{
	if (sigid == SIGINT)
	{
		write(STDERR_FILENO, "\n", 1);
		rl_on_new_line();
		rl_replace_line("", 0);
		rl_redisplay();
	}
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
