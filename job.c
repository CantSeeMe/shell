/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   job.c                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jye <jye@student.42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2017/07/13 16:07:52 by jye               #+#    #+#             */
/*   Updated: 2017/09/14 16:09:08 by root             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "lst.h"
#include "job.h"
#include "command.h"
#include "token.h"
#include "etc_parse.h"
#include "ft_readline.h"

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

char	**set_envp(void)
{
	int		i;
	t_lst	*env;
	char	**envp;
	t_var	*v;

	vhash_set_underscore(HTVAR_SET_PATH, c);
	if ((envp = malloc(sizeof(char *) * (g_envpsize + 1))) == 0)
		return (0);
	i = g_envpsize;
	envp[i--] = 0;
	env = g_envp;
	while (env)
	{
		v = env->data;
		envp[i--] = defrag_var(v->key, v->value);
		envp = envp->next;
	}
	return (envp);
}

int		test_execpath(char *pname, char *c)
{
	struct stat stats;

	if (stat(c, &stats))
	{
		exec_error(pname, c, 1);
		exit(127);
	}
	if (S_ISDIR(stats.st_mode))
	{
		exec_error(pname, c, 2);
		exit(126);
	}
	if (!(stats.st_mode & (S_IXUSR | S_IXGRP | S_IXOTH)))
	{
		exec_error(pname, c, 3);
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
		exec_error("minishell", rd->fd_.s, 0);
		exit(1);
	}
	dup2(fd, rd->fd_.fd);
}

void	job_heretag(t_rdtype *rd)
{
	if (rd->fd_.heretag == -2)
		return ;
	dup2(rd->fd_.heretag, rd->fd_.fd);
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
				job_heretag(rd);
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

int		job_child_norm(t_command *c)
{
	int	i;
	if (c->var_ && c->cmd.c == 0)
		exit(0);
	job_openfds(c->redir);
	i = c->var_;
	while (i < c->ac)
		givemeback_letter_pls(c->av.cav[i++]);
	if (c->cmd.type == C_SHELL_BUILTIN)		
		return (((t_builtin)c->cmd.c)(c->ac - c->var_, c->av.cav + c->var_, c->envp) << 8);
	if (c->cmd.c == 0)
	{
		exec_error("minishell", *(c->av.cav + c->var_), 0);
		exit(127);
	}
	test_execpath(c->cmd.c);
	return (execve(c->cmd.c, c->av.cav + c->var_, c->envp));
}

int		job_fork_child(t_lst *c, int endsym)
{
	pid_t	pid;
	int		status;

	pid = fork();
	if (pid == 0)
	{
		if (endsym == pip)
			job_child_pipe(c);
		else
			job_child_norm((t_command *)c->data);
	}
	if (pid > 0)
		waitpid(pid, &status, WUNTRACED);
	return (status);
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

int		job_father_cond(t_lst *job)
{
	int			status;
	t_command	*c;

	while (job)
	{
		c = job->data;
		if (c->endsym == pip || c->cmd.type == C_SHELL_EXT)
			status = job_fork_child(job, c->endsym);
		else
			status = job_child_norm(c);
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
	return (status);
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
		if (c->cmd.type == C_SHELL_EXT)
		{
			pid = fork();
			if (pid == 0)
				job_child_norm(c);
			if (pid > 0)
				waitpid(pid, &status, WUNTRACED);
		}
		else
			status = job_child_norm(c);
		dup2(fd[0], 0);
		close(fd[1]);
		job = job->next;
		c = job->data;
	}
	dup2(fdsave, 1);
	close(fd[1]);
	status = job_child_norm(c);
	exit(WEXITSTATUS(status));
}

void	job_father(t_job *job)
{
	int		status;

	if (job->type & JTNOHANG)
		g_jobs = (t_lst *)-1;
	if (job->type & JTCOND)
		status = job_father_cond(job->c);
	else if (job->type & JTPIPE)
		job_child_pipe(job->c);
	else
		status = job_child_norm((t_command *)job->c->data);
	exit(WTERMSIG(status));
}

void	job_grandfather(t_job *job)
{
	sigset_t	set;

	sigemptyset(&set);
	sigaddset(&set, SIGTSTP);
	sigaddset(&set, SIGINT);
	if (job->type == JTNOHANG)
	{
		push_lst__(&g_jobs, job);
	}
	else
	{
		if (job->pid > 0)
			waitpid(job->pid, &job->type, WUNTRACED);
		job_setlaststatus(job, job->type);
	}
	sigprocmask(SIG_BLOCK, &set, NULL);
	sigprocmask(SIG_UNBLOCK, &set, NULL);
}

void	job_exec(t_job *job)
{
	pid_t 		pid;

	if (job == 0)
		return ;
	pid = fork();
	if (pid == 0)
	{
		job_father(job);
	}
	job->pid = pid;
	job_grandfather(job);
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
		job->type |= (z->endsym == pip ? JTPIPE : JTCOND) |
			         (z->cmd.type == C_SHELL_EXT ? 0 : JTBUILTIN);
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

void	job_builtinexec(t_job *job)
{
	int			status;

	status = -1;
	if (job == 0)
		return ;
	if (job->type & JTNOHANG)
		job_exec(job);
	else if (job->type & JTCOND)
		status = job_father_cond(job->c);
	else if (job->type & JTPIPE)
		job_exec(job);
	else
		status = job_child_norm(job->c->data);
	if (status > -1)
		job_setlaststatus(job, status);
}

void	job_setlaststatus(t_job *job, int status)
{
	if (WIFSIGNALED(status))
	{
		if (WTERMSIG(status) == SIGTSTP)
			push_lst__(&g_jobs, job);
		g_laststatus = WTERMSIG(status) + 128;
	}
	else
		g_laststatus = WEXITSTATUS(status);
}

char	*job_retlaststatus(void)
{
	int			l;
	char		*itoa_ptr;
	static char	status[4];

	if (!g_laststatus)
	{
		status[0] = 0x30;
		status[1] = 0;
		return (status);
	}
	l = g_laststatus;
	itoa_ptr = status;
	while (l)
	{
		l /= 10;
		itoa_ptr++;
	}
	*itoa_ptr-- = 0;
	l = g_laststatus;
	while (l)
	{
		*itoa_ptr-- = 0x30 + (l % 10);
		l /= 10;
	}
	return (status);
}
