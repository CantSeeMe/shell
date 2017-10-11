/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   job8.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: root <marvin@42.fr>                        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2017/10/02 22:42:01 by root              #+#    #+#             */
/*   Updated: 2017/10/10 23:14:17 by root             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "job.h"
#include "htvar.h"
#include "command.h"
#include "ft_printf.h"

#include <stdlib.h>
#include <unistd.h>
#include <signal.h>
#include <sys/wait.h>

char		*job_getstatus(void)
{
	int			l;
	char		*itoa_ptr;
	static char	status[4];

	if (!g_js.pstat)
	{
		status[0] = 0x30;
		status[1] = 0;
		return (status);
	}
	l = g_js.pstat;
	itoa_ptr = status;
	while (l)
	{
		l /= 10;
		itoa_ptr++;
	}
	*itoa_ptr-- = 0;
	l = g_js.pstat;
	while (l)
	{
		*itoa_ptr-- = 0x30 + (l % 10);
		l /= 10;
	}
	return (status);
}

int		job_wait_control_(pid_t pid, int options)
{
	struct sigaction	act;
	int					status;

	sigemptyset(&act.sa_mask);
	/* sigaddset(&act.sa_mask, SIGINT); */
	/* sigaddset(&act.sa_mask, SIGTSTP); */
	/* sigaddset(&act.sa_mask, SIGTTIN); */
	/* sigaddset(&act.sa_mask, SIGTTOU); */
	act.sa_handler = SIG_IGN;
	act.sa_flags = 0;
	/* sigprocmask(SIG_BLOCK, &act.sa_mask, NULL); */
	sigaction(SIGINT, &act, 0);
	sigaction(SIGTSTP, &act, 0);
	sigaction(SIGTTIN, &act, 0);
	sigaction(SIGTTOU, &act, 0);
	status = 127 << 16;
	if (pid > 0)
		waitpid(pid, &status, options);
	/* sigprocmask(SIG_UNBLOCK, &act.sa_mask, NULL); */
	act.sa_handler = SIG_DFL;
	sigaction(SIGINT, &act, 0);
	sigaction(SIGTSTP, &act, 0);
	sigaction(SIGTTIN, &act, 0);
	sigaction(SIGTTOU, &act, 0);
	return (status);
}

int			job_wait_control(t_process *proc, int options)
{
	int			status;

	status = job_wait_control_(proc->pid, options);
	proc->state = ((JT_IS_NOHANG(status) * JT_BACKGROUND) |
				(WIFSTOPPED(status) * JT_SUSPENDED));
	if (proc->state)
		job_insert_to_list(proc);
	else
	{
		free_full_parsed_command(proc->c);
		free(proc);
	}
	if ((options & WNOHANG))
		return (status);
	g_js.exit = 0;
	if (WTERMSIG(status))
		return (g_js.pstat = WTERMSIG(status) + 128);
	return (g_js.pstat = WEXITSTATUS(status));
}

void		job_exec(t_job *job)
{
	if (job->type & JTCOND)
	{
		job_cond_fork(&job->proc, job->type & JTNOHANG);
	}
	else if (job->type & JTPIPE)
	{
		job_pipe_fork(&job->proc, job->type & JTNOHANG);
	}
	else
	{
		job_fork_alone(&job->proc, job->type & JTNOHANG);
	}
	free(job);
}
