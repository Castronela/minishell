/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   signal.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pamatya <pamatya@student.42heilbronn.de    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/19 17:35:32 by dstinghe          #+#    #+#             */
/*   Updated: 2024/12/25 14:57:00 by pamatya          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

# include "minishell.h"
# include <signal.h>
# include <termios.h>

void set_signal(t_shell *shell);
static void signal_handler(int sigid, siginfo_t *info, void *ucontext);

void set_signal(t_shell *shell)
{
    struct sigaction sa;
    struct termios term;
    
    sa.sa_sigaction = signal_handler;
    sigemptyset(&sa.sa_mask);
    sa.sa_flags = SA_SIGINFO | SA_RESTART;
    sigaddset(&sa.sa_mask, SIGINT);
    signal(SIGQUIT, SIG_IGN);
    if (sigaction(SIGINT, &sa, NULL) == -1)
        exit_early(shell, NULL, ERRMSG_SIGNAL);
    tcgetattr(STDIN_FILENO, &term);
    term.c_lflag &= ~ECHOCTL;
    tcsetattr(STDIN_FILENO, TCSANOW, &term);
}

static void signal_handler(int sigid, siginfo_t *info, void *ucontext)
{
    (void)info;
    (void)ucontext;
    if (sigid ==SIGINT)
    {
        write(1, "\n", 1);
        rl_on_new_line();
        rl_replace_line("", 0);
        rl_redisplay();
    }
}
