/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   job.c                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jye <jye@student.42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2017/07/13 16:07:52 by jye               #+#    #+#             */
/*   Updated: 2017/08/25 09:01:20 by jye              ###   ########.fr       */
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
#include <sys/types.h>
#include <sys/stat.h>
#include <sys/wait.h>


t_lst	*g_jobs;
int		g_laststatus;

int		test_execpath(char *c)
{
	struct stat stats;

	if (c == 0)
		return (0);
	if (!strchr(c, '/'))
	{
		exec_error(c, 0);
		exit(127);
	}
	if (stat(c, &stats))
	{
		exec_error(c, 1);
		exit(127);
	}
	if (S_ISDIR(stats.st_mode))
	{
		exec_error(c, 2);
		exit(126);
	}
	if (!(stats.st_mode & (S_IXUSR | S_IXGRP | S_IXOTH)))
	{
		exec_error(c, 3);
		exit(126);
	}
	return (0);
}

/* void	job_openfds(t_lst *redir) */
/* { */

/* } */

void	job_child_norm(t_command *c)
{
	if (c->cmd.c == 0)
		exit(0);
//	job_openfds(c->redir);
	if (c->cmd.type == C_SHELL_BUILTIN)
		((t_builtin)c->cmd.c)(c->ac, c->av.cav, c->envp);
	test_execpath(c->cmd.c);
	execve(c->cmd.c, c->av.cav, c->envp);
}

void	job_child_cond(t_lst *c, int endsym)
{
	if (endsym == pip)
		job_child_pipe(c);
	else
		job_child_norm((t_command *)c->data);
}

int		and_check(int status)
{
	return (!WTERMSIG(status) && !WEXITSTATUS(status));
}

int		or_check(int status)
{
	return (WTERMSIG(status) && WEXITSTATUS(status));
}

void	job_skipsym(t_lst **job, int sym)
{
	t_lst		*c;
	t_command	*co;

	c = (*job)->next;
	while (c)
	{
		co = c->data;
		if (co->endsym == sym)
			break ;
		c = c->next;
	}
	*job = c;
}

void	job_father_cond(t_lst *job)
{
	pid_t		pid;
	int			status;
	t_command	*c;

	while (job)
	{
		c = job->data;
		pid = fork();
		if (pid == 0)
			job_child_cond(job, c->endsym);
		waitpid(pid, &status, WUNTRACED);
		if (c->endsym == andsym && and_check(status))
			job_skipsym(&job, orsym);
		else if (c->endsym == orsym && or_check(status))
			job_skipsym(&job, andsym);
	}
}

void	job_child_pipe(t_lst *job)
{
	pid_t		pid;
	int			fdsave[2];
	int			fd[2];
	int			status;
	t_command	*c;

	fdsave[0] = dup(0);
	fdsave[1] = dup(1);
	fd[0] = 3;
	pipe(fd);
	dup2(fd[1], 0);
	dup2(fd[0], 1);
	c = job->data;
	while (c->endsym == pip)
	{
		pid = fork();
		if (pid == 0)
			job_child_norm(c);
		waitpid(pid, &status, WUNTRACED);
		if (WTERMSIG(status))
			;
		job = job->next;
		c = job->data;
	}
	dup2(fdsave[1], 1);
//	close(fd[0]);
//	close(fd[1]);
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

/* /\**reference**\/ */
/* int		main(int ac, char **av, char **envp) */
/* { */
/* 	pid_t	id; */
/* 	char	*smth[] = {"ls", "-R", getenv("HOME"), NULL}; */
/* 	int		status; */
/* 	sigset_t	set; */

/* 	sigemptyset(&set); // initialize sigset */
/* 	sigaddset(&set, SIGTSTP); // adding some mask */
/* 	sigaddset(&set, SIGINT); */

/* 	id = fork(); */
/* 	if (id == 0) /\* child *\/ */
/* 	{ */
/* //		sigprocmask(SIG_UNBLOCK, &set, NULL); // unblock signal on current process */
/* 		return (execve("/bin/ls", smth, envp)); */
/* 	} */
/* 	else */
/* 	{ */
/* 		/\* father *\/ */
/* 		sigprocmask(SIG_BLOCK, &set, NULL); // block signal on current process */
/* 		waitpid(id, &status, WUNTRACED); */
/* 		dprintf(2, "%d %d %d %d %s\n", WEXITSTATUS(status), WTERMSIG(status), */
/* 				WCOREDUMP(status), WSTOPSIG(status), strerror(errno)); */
/* 		if (WIFSTOPPED(status)) */
/* 		{ */
/* 			kill(id, SIGCONT); */
/* 		} */
/* 	} */
/* 	waitpid(id, &status, 0); */
/* 	dprintf(2, "%d %d %s\n", WEXITSTATUS(status), id, strerror(errno)); */
/* 	/\* WNOHANG for no pchild process wait *\/ */
/* } */
