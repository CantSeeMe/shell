/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   job10.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: root <marvin@42.fr>                        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2017/10/11 13:03:17 by root              #+#    #+#             */
/*   Updated: 2017/10/16 17:19:45 by root             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "job.h"
#include "ft_printf.h"

#include <sys/wait.h>
#include <signal.h>

void	init_sig_string2(void)
{
	g_sig_[SIGPIPE] = "Broken pipe";
	g_sig_[SIGALRM] = "Alarm clock";
	g_sig_[SIGTERM] = "Terminated";
	g_sig_[SIGURG] = "Urgent IO condition";
	g_sig_[SIGSTOP] = "Stopped (signal)";
	g_sig_[SIGTSTP] = "Stopped";
	g_sig_[SIGCONT] = "Continue";
	g_sig_[SIGCHLD] = "Child death or stop";
	g_sig_[SIGTTIN] = "Stopped (tty input)";
	g_sig_[SIGTTOU] = "Stopped (tty output)";
	g_sig_[SIGIO] = "I/O ready";
	g_sig_[SIGXCPU] = "CPU limit";
}

void	init_sig_string(void)
{
	int	i;

	i = 0;
	while (i < NOSIG)
		g_sig_[i++] = "unknow signal";
	g_sig_[0] = "Exited";
	g_sig_[SIGHUP] = "Hangup";
	g_sig_[SIGINT] = "Interrupt";
	g_sig_[SIGQUIT] = "Quit";
	g_sig_[SIGILL] = "Illegal Instruction";
	g_sig_[SIGTRAP] = "Trapped";
	g_sig_[SIGABRT] = "Aborted";
	g_sig_[SIGFPE] = "Floating point exception";
	g_sig_[SIGKILL] = "Killed";
	g_sig_[SIGBUS] = "Bus error";
	g_sig_[SIGSEGV] = "Segmentation Fault";
	g_sig_[SIGSYS] = "Bad system call";
	init_sig_string2();
}

void	job_print_process_status(t_process *proc, int qid, char *s)
{
	ft_dprintf(1, "[%d] %s%s%s %d %s\n", qid + 1,
			qid == g_js.cur ? "+ " : "",
			qid == g_js.prev ? "- " : "",
			proc->c ? *(proc->c->av.cav + proc->c->var_) : "", proc->pid,
			s);
}
