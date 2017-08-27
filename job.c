/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   job.c                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jye <jye@student.42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2017/07/13 16:07:52 by jye               #+#    #+#             */
/*   Updated: 2017/08/27 02:35:57 by jye              ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "lst.h"
#include "job.h"
#include "command.h"
#include "token.h"

#include <stdlib.h>
#include <unistd.h>
#include <stdio.h>
#include <signal.h>
#include <errno.h>
#include <string.h>
#include <fcntl.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <sys/wait.h>


t_lst	*g_jobs;
int		g_laststatus;

int		test_execpath(t_command *c)
{
	struct stat stats;

	if (c->cmd.c == 0)
	{
		exec_error(*(c->av.cav + c->var_), 0);
		exit(127);
	}
	if (stat(c->cmd.c, &stats))
	{
		exec_error(c->cmd.c, 1);
		exit(127);
	}
	if (S_ISDIR(stats.st_mode))
	{
		exec_error(c->cmd.c, 2);
		exit(126);
	}
	if (!(stats.st_mode & (S_IXUSR | S_IXGRP | S_IXOTH)))
	{
		exec_error(c->cmd.c, 3);
		exit(126);
	}
	return (0);
}

void	job_openstdin(t_rdtype *rd)
{
	int	fd;

	fd = open(rd->fd_.s, rd->fd_.o_flag);
	if (fd == -1)
	{
		exec_error(rd->fd_.s, 1);
		exit(1);
	}
	dup2(fd, rd->fd_.fd);
}

void	job_openfds(t_lst *redir)
{
	t_rdtype	*rd;
	int			fd;

	if (redir == 0)
		return ;
	while (redir)
	{
		rd = (t_rdtype *)redir->data;
		if (rd->type == RDF_FDREDIR)
			dup2(atoi(rd->fd_.s), rd->fd_.fd);
		else if (rd->type == RDF_STDIN)
		{
			if (rd->fd_.o_flag == -1)
				;
			else
				job_openstdin(rd);
		}
		else
		{
			fd = open(rd->fd_.s, rd->fd_.o_flag, 0644);
			dup2(fd, rd->fd_.fd);
		}
		redir = redir->next;
	}
}

void	job_child_norm(t_command *c)
{
	if (c->var_ && c->cmd.c == 0)
		exit(0);
	job_openfds(c->redir);
	if (c->cmd.type == C_SHELL_BUILTIN)		
		((t_builtin)c->cmd.c)(c->ac, c->av.cav, c->envp);
	test_execpath(c);
	execve(c->cmd.c, c->av.cav + c->var_, c->envp);
}

void	job_child_cond(t_lst *c, int endsym)
{
	if (endsym == pip)
		job_child_pipe(c);
	else
		job_child_norm((t_command *)c->data);
}

int		status_check(int status)
{
	return (WTERMSIG(status) || WEXITSTATUS(status));
}

void	job_skipsym(t_lst **job, int sym)
{
	t_lst		*c;
	t_command	*co;

	c = (*job);
	while (c)
	{
		co = c->data;
		if (co->endsym == sym)
			break ;
		c = c->next;
	}
	*job = c ? c->next : 0;

}

void	job_father_cond(t_lst *job)
{
	pid_t		pid;
	int			status;
	t_command	*c;

	while (job)
	{

		c = job->data;
		dprintf(2, "%s\n", c->endsym == andsym ? "andsym" :
						   c->endsym == orsym ? "orsym" : "pipe");
		pid = fork();
		if (pid == 0)
			job_child_cond(job, c->endsym);
		waitpid(pid, &status, WUNTRACED);
		if (c->endsym == pip)
			while (job)
			{
				if (((t_command *)job->data)->endsym != pip)
					break ;
				job = job->next;
			}
		job = job->next;
		if (c->endsym == andsym && status_check(status))
			job_skipsym(&job, orsym);
		if (c->endsym == orsym && !status_check(status))
			job_skipsym(&job, andsym);
	}
}

void	job_child_pipe(t_lst *job)
{
	pid_t		pid;
	int			fdsave;
	int			fd[2];
	int			status;
	t_command	*c;

	fdsave = dup(1);
	c = job->data;
	while (c->endsym == pip)
	{
		pipe(fd);
		dup2(fd[1], 1);
		pid = fork();
		if (pid == 0)
			job_child_norm(c);
		waitpid(pid, &status, WUNTRACED);
		dup2(fd[0], 0);
		close(fd[1]);
		job = job->next;
		c = job->data;
	}
	dup2(fdsave, 1);
	close(fd[1]);
	job_child_norm(c);
}

void	job_father(t_job *job)
{
	if (job->type & JTCOND)
		job_father_cond(job->c);
	else if (job->type & JTPIPE)
		job_child_pipe(job->c);
	else
		job_child_norm((t_command *)job->c->data);		
}

void	job_grandfather(t_job *job)
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
			g_laststatus = WEXITSTATUS(job->type);
	}
}

void	job_exec(t_job *job)
{
	pid_t 		pid;
	sigset_t	set;

//	job_check();
	if (job == 0)
		return ;
	pid = fork();
	if (pid == 0)
	{
		job_father(job);
	}
	else
	{
		sigemptyset(&set);
		sigaddset(&set, SIGTSTP);
		sigaddset(&set, SIGINT);
		sigprocmask(SIG_BLOCK, &set, NULL);
		job->pid = pid;
		job_grandfather(job);
		sigprocmask(SIG_UNBLOCK, &set, NULL);
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
	job->type = 0;
	while (cp)
	{
		z = cp->data;
		if (z->endsym == ampersand || z->endsym == period)
			break ;
		job->type |= z->endsym == pip ? JTPIPE : JTCOND;
		cp = cp->next;
	}
	job->type |= z->endsym == ampersand ? JTNOHANG : JTHANG;
	job->c = *c;
	job->id = g_jobs ? ((t_job *)g_jobs->data)->id + 1 : 1;
	if (cp)
	{
		*c = cp->next;
		cp->next = 0;
	}
	return (job);
}
