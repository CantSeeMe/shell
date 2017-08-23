/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   job.c                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jye <jye@student.42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2017/07/13 16:07:52 by jye               #+#    #+#             */
/*   Updated: 2017/08/23 18:28:50 by root             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "lst.h"
#include "job.h"
#include "command.h"

#include <stdlib.h>
#include <unistd.h>
#include <stdio.h>
#include <signal.h>
#include <errno.h>
#include <string.h>

t_lst	*g_jobs;
int		g_laststatus;

void	job_father_exec(t_job *job)
{
	if (job->type == JTNOHANG)
	{
		push_lst__(&g_jobs, job);
	}
	else
	{
		waitpid(job->pid, &job->type, WUNTRACED);
		if (WIFSIGNALED(job->type))
		{
			if (WTERMSIG(job->type) == SIGTSTP)
				push_lst__(&g_jobs, job);
			g_laststatus = WTERMSIG(job->type) + 128;
		}
		else
			g_laststatus = WEXITEDSTATUS(job->type);
	}
}

void	job_child_exec(t_lst *c)
{
	t_command	*cc;

	while (c)
	{
		cc = (t_command *)c->data;
		cc->
	}
}

void	job_exec(t_job *job)
{
	pid_t 		pid;
	sigset_t	set;

	job_check();
	if (job == 0)
		return ;
	pid = fork();
	if (pid == 0)
	{
		job_child_exec(job->c);
	}
	else
	{
		sigemptyset(&set); // initialize sigset
		sigaddset(&set, SIGTSTP); // adding some mask
		sigaddset(&set, SIGINT);
		sigprocmask(SIG_BLOCK, &set, NULL); // block signal on current process
		job->pid = pid;
		job_father_exec(job);
		sigprocmask(SIG_UNBLOCK, &set, NULL); // block signal on current process
	}
}

t_job	*job_create(t_lst **c)
{
	t_job		*job;
	t_lst		*cp;
	t_command	*z;

	if ((job = malloc(sizeof(*job))) == 0)
		return ((t_job *)0);
	cp = *c;
	while (cp)
	{
		z = cp->data;
		if (z->endsym == ampersand || z->endsym == period)
			break ;
		cp = cp->next;
	}
	job->type = z->sym == ampersand ? JTNOHANG : JTHANG;
	job->c = *c;
	job->id = g_jobs ? ((t_job *)g_jobs->data)->id + 1 : 1;
	if (cp)
	{
		*c = cp->next;
		cp->next = NULL;
	}
	return (job);
}

/**reference**/
int		main(int ac, char **av, char **envp)
{
	pid_t	id;
	char	*smth[] = {"ls", "-R", getenv("HOME"), NULL};
	int		status;
	sigset_t	set;

	sigemptyset(&set); // initialize sigset
	sigaddset(&set, SIGTSTP); // adding some mask
	sigaddset(&set, SIGINT);

	id = fork();
	if (id == 0) /* child */
	{
//		sigprocmask(SIG_UNBLOCK, &set, NULL); // unblock signal on current process
		return (execve("/bin/ls", smth, envp));
	}
	else
	{
		/* father */
		sigprocmask(SIG_BLOCK, &set, NULL); // block signal on current process
		waitpid(id, &status, WUNTRACED);
		dprintf(2, "%d %d %d %d %s\n", WEXITSTATUS(status), WTERMSIG(status),
				WCOREDUMP(status), WSTOPSIG(status), strerror(errno));
		if (WIFSTOPPED(status))
		{
			kill(id, SIGCONT);
		}
	}
	waitpid(id, &status, 0);
	dprintf(2, "%d %d %s\n", WEXITSTATUS(status), id, strerror(errno));
	/* WNOHANG for no pchild process wait */
}
