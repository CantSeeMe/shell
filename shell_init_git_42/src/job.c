/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   job.c                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jye <jye@student.42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2017/07/13 16:07:52 by jye               #+#    #+#             */
/*   Updated: 2017/09/25 13:39:08 by root             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "lst.h"
#include "job.h"
#include "htvar.h"
#include "command.h"
#include "ft_printf.h"

#include <stdlib.h>
#include <unistd.h>
#include <signal.h>
#include <sys/wait.h>

t_lst	*g_jobs;
int		g_laststatus;

char		*job_getstatus(void)
{
	int			l;
	char		*itoa_ptr;
	static char	status[4];

	if (!(g_laststatus & 0xff))
	{
		status[0] = 0x30;
		status[1] = 0;
		return (status);
	}
	l = g_laststatus & 0xff;
	itoa_ptr = status;
	while (l)
	{
		l /= 10;
		itoa_ptr++;
	}
	*itoa_ptr-- = 0;
	l = g_laststatus & 0xff;
	while (l)
	{
		*itoa_ptr-- = 0x30 + (l % 10);
		l /= 10;
	}
	return (status);
}

static int	job_wait_control_(pid_t pid, int options)
{
	struct sigaction	act;
	int					status;

	sigemptyset(&act.sa_mask);
	sigaddset(&act.sa_mask, SIGTSTP);
	sigaddset(&act.sa_mask, SIGINT);
	act.sa_handler = SIG_IGN;
	act.sa_flags = 0;
	sigaction(SIGINT, &act, 0);
	sigaction(SIGTSTP, &act, 0);
	status = 127 << 8;
	if (pid > 0)
		waitpid(pid, &status, options);
	act.sa_handler = SIG_DFL;
	sigaction(SIGINT, &act, 0);
	sigaction(SIGTSTP, &act, 0);
	return (status);
}

int			job_wait_control(t_process *proc, int options)
{
	int			status;

	status = job_wait_control_(proc->pid, options);
	if ((proc->flag = ((options & WNOHANG) |
						((WIFSTOPPED(status) * WTERMSIG(status)) << 16))))
	{
		proc->qid = g_jobs ? ((t_process *)g_jobs->data)->qid + 1 : 1;
		push_lst__(&g_jobs, proc);
	}
	else
	{
		free_full_parsed_command(proc->c);
		free(proc);
	}
	if ((options & WNOHANG))
		return (status);
	g_laststatus &= ~0xffff;
	if (WTERMSIG(status))
		return (g_laststatus |= WTERMSIG(status) + 128);
	return (g_laststatus |= WEXITSTATUS(status));
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
