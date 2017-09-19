/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   job.c                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jye <jye@student.42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2017/07/13 16:07:52 by jye               #+#    #+#             */
/*   Updated: 2017/09/19 00:42:31 by root             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "lst.h"
#include "job.h"
#include "htvar.h"
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




int		transmute_av(t_command *c)
{
	char	**av;
	char	*dollar;
	t_lst	*z;
	int		i;

	if ((av = malloc(sizeof(*av) * (c->ac + 1))) == 0)
		return (1);
	z = c->av.lav;
	i = c->ac;
	av[i--] = 0;
	while (z)
	{
		dollar = transmute_dollar((char *)z->data);
		av[i--] = dollar ? dollar : z->data;
		pop_lst__(&z, dollar ? free : 0);
	}
	c->av.cav = av;
	return (0);
}

/******************************/
/******************************/
/******************************/

#define WORD_UCHAR	"ABCDEFGHIJKLMNOPQRSTUVWXYZ"
#define WORD_LCHAR	"abcdefghijklmnopqrstuvwxyz"
#define WORD_SCHAR	"._-"
#define WORD_NCHAR	"1234567890"

#define WORD_ANCHAR	WORD_UCHAR WORD_LCHAR WORD_NCHAR

int		check_varname(char *s)
{
	while (*s != '=')
	{
		if (!strchr(WORD_ANCHAR, *s))
			return (0);
		s++;
	}
	return (1);
}

int		set_execpath(t_command *c)
{
	t_ccsh		*z;
	t_var		*v;

	if (strchr(*c->av.cav, '=') && check_varname(*c->av.cav))
	{
		if ((v = init_var(*c->av.cav, HTVAR_VAR_GLOBAL)))
			vhash_insert(v);
		c->var_ = 1;
	}
	else
	{
		c->var_ = 0;
	}
	c->cmd.type = C_SHELL_EXT;
	if (strchr(*(c->av.cav + c->var_), '/'))
		c->cmd.c = *(c->av.cav + c->var_);
	else if ((z = chash_lookup(*(c->av.cav + c->var_), vhash_search("PATH"))))
		c->cmd = *z;
	else
		c->cmd.c = 0;
	return (0);
}




char	**set_envp(void)
{
	int		i;
	t_lst	*env;
	char	**envp;
	t_var	*v;

	if ((envp = malloc(sizeof(char *) * (g_envpsize + 1))) == 0)
		return (0);
	i = g_envpsize;
	envp[i--] = 0;
	env = g_envp;
	while (env)
	{
		v = env->data;
		envp[i--] = defrag_var(v->key, v->value);
		env = env->next;
	}
	return (envp);
}

int		test_execpath(char *c)
{
	struct stat stats;

	if (c == NULL)
		return (1);
	if (stat(c, &stats))
		return (2);
	if (S_ISDIR(stats.st_mode))
		return (3);
	if (!(stats.st_mode & (S_IXUSR | S_IXGRP | S_IXOTH)))
		return (4);
	return (0);
}

char	*job_getstatus(void)
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
	l = g_laststatus & 0xff;
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

int		job_wait_control(t_process *proc, int options)
{
	int			status;
	sigset_t	set;

	sigemptyset(&set);
	sigaddset(&set, SIGTSTP);
	sigaddset(&set, SIGINT);
	sigprocmask(SIG_BLOCK, &set, NULL);
	status = 127 << 8;
	if (proc->pid > 0)
		waitpid(proc->pid, &status, options);
	sigprocmask(SIG_UNBLOCK, &set, NULL);
	if ((proc->flag = ((options & WNOHANG) |
					   ((WIFSTOPPED(status) * WTERMSIG(status)) << 16))))
		push_lst__(&g_jobs, proc);
	else
	{
		free_full_parsed_command(proc->c);
		free(proc);
	}
	if ((options & WNOHANG))
		return (status);
	else if (WTERMSIG(status))
		return (g_laststatus = WTERMSIG(status) + 128);
	else
		return (g_laststatus = WEXITSTATUS(status));
}

void	job_outopen(t_rd *rd)
{
	int		fd;

	if ((fd = open(rd->s, rd->o_flag, 0644)) == -1)
	{
		free(rd->s);
		rd->save = -1;
		return ;
	}
	free(rd->s);
	rd->save = dup(rd->fd);
	dup2(fd, rd->fd);
	close(fd);
}

void	job_inopen(t_rd *rd)
{
	int			fd;

	if (rd->s == 0 && rd->o_flag == -1)
	{
		if (rd->heretag == -2)
			return ;
		rd->save = dup(rd->fd);
		dup2(rd->heretag, rd->fd);
		close(rd->heretag);
		rd->heretag = -1;
	}
	else
	{
		if ((fd = open(rd->s, rd->o_flag)) == -1)
		{
			free(rd->s);
			return ;
		}
		free(rd->s);
		rd->save = dup(rd->fd);
		dup2(fd, rd->fd);
		close(fd);
	}
}

void	job_rdiropen(t_rd *rd)
{
	int	fd;

	fd = rd->s ? atoi(rd->s) : -1;
	free(rd->s);
	if (fd == -1)
		return ;
	rd->save = dup(rd->fd);
	dup2(fd, rd->fd);
}


t_lst	*job_openfd(t_lst *redir)
{
	t_rd	*rd;
	t_lst	*rest;

	rest = 0;
	while (redir)
	{
		rd = (t_rd *)redir->data;
		pop_lst__(&redir, 0);
		push_lst__(&rest, rd);
		if (rd->type == RDF_OUT)
		{
			job_outopen(rd);
		}
		else if (rd->type == RDF_IN)
		{
			job_inopen(rd);
		}
		else if (rd->type == RDF_RDIR)
		{
			job_rdiropen(rd);
		}
	}
	return (rest);
}

void	job_restorefd(t_lst *rest)
{
	t_rd	*rd;

	while (rest)
	{
		rd = (t_rd *)rest->data;
		dup2(rd->save, rd->fd);
		close(rd->save);
		pop_lst__(&rest, free);
	}
}

void	job_fork_exec(t_command *c)
{
	int	i;

	if (c->var_ && c->av.cav[1] == 0)
		exit(0);
	if ((i = test_execpath(c->cmd.c)))
	{
		exec_error("minishell", *(c->av.cav + c->var_), i - 1);
		exit(126 + (i <= 2));
	}
	exit(execve((const char *)c->cmd.c, c->av.cav + c->var_, c->envp));
}

int		job_exec_process(t_process *p)
{
	t_command	*c;
	t_lst		*rest;
	int			i;

	c = p->c;
	i = c->var_;
	while (i < c->ac)
		givemeback_letter_pls(c->av.cav[i++]);
	rest = job_openfd(p->c->redir);
	if (c->cmd.type == C_SHELL_BUILTIN)
	{
		if (p->pid == 0)
			exit(((t_builtin)c->cmd.c)(c->ac - c->var_,
									   c->av.cav + c->var_,
									   c->envp));
		g_laststatus = (((t_builtin)c->cmd.c)
						(c->ac - c->var_,
						 c->av.cav + c->var_,
						 c->envp));
		job_restorefd(rest);
		return (g_laststatus);
	}
	job_fork_exec(c);
	return (127 << 8);
}

void	free_half_parsed_command(t_command *c)
{
	t_rd	*rd;

	while (c->av.lav)
		pop_lst__(&c->av.lav, free);
	while (c->redir)
	{
		rd = (t_rd *)c->redir->data;
		free(rd->s);
		if (rd->heretag > 0)
			close(rd->heretag);
		pop_lst__(&c->redir, free);
	}
	free(c);
}

void	free_full_parsed_command(t_command *c)
{
	char	**d;
	t_rd	*rd;

	d = c->av.cav;
	while (*d)
		free(*d++);
	free(c->av.cav);
	d = c->envp;
	while (*d)
		free(*d++);
	free(c->envp);
	while (c->redir)
	{
		rd = (t_rd *)c->redir->data;
		free(rd->s);
		if (rd->heretag > 0)
			close(rd->heretag);
		pop_lst__(&c->redir, free);
	}
	free(c);
}

void	dummy_job_create(t_lst **command)
{
	t_command	*c;
	t_lst		*l;
	int			sym;

	l = *command;
	while (l)
	{
		c = (t_command *)l->data;
		sym = c->endsym;
		pop_lst__(&l, free_half_parsed_command);
		if (sym == period || sym == ampersand)
			break ;
	}
	*command = l;
}

void	job_test_malloc(t_job *job, t_lst **cp)
{
	void	*t;

	while (*cp && (t = malloc(sizeof(t_process))) == 0)
		pop_lst__(cp, free_half_parsed_command);
	free(t);
	job->proc = *cp;
}

t_lst	*job_create_process(t_job *job, t_lst *cp)
{
	t_command	*c;
	t_process	*p;

	job_test_malloc(job, &cp);
	while (cp)
	{
		c = (t_command *)cp->data;
		if ((p = malloc(sizeof(*p))) == 0)
		{
			pop_lst__(&cp, free_half_parsed_command);
			continue ;
		}
		transmute_av(c);
		set_execpath(c);
		dprintf(1, "%s\n", *c->av.cav);
		vhash_set_underscore(c->cmd.type == C_SHELL_BUILTIN ?
							 c->cmd.key : c->cmd.c);
		c->envp = set_envp();
		p->qid = g_jobs ? ((t_process *)g_jobs->data)->qid + 1 : 1;
		p->pid = -1;
		job->type = (c->endsym == pip ? JTPIPE : JTCOND) |
			        (c->cmd.type == C_SHELL_BUILTIN ? JTBUILTIN : 0);
		cp->data = p;
		p->c = c;
		if (c->endsym == ampersand || c->endsym == period)
			break ;
		cp = cp->next;
	}
	return (cp ? cp->next : cp);
}

t_job	*job_create(t_lst **command)
{
	t_job		*job;
	t_lst		*smh;

	if (*command == (t_lst *)0 ||
		(job = malloc(sizeof(*job))) == 0)
	{
		dummy_job_create(command);
		return ((t_job *)0);
	}
	job->type = 0;
	*command = job_create_process(job, *command);
	if (*command)
	{
		smh = (*command)->prev;
		(*command)->prev = (t_lst *)0;
		smh->next = (t_lst *)0;
	}
	return (job);
}

int		job_pipe_wait(t_lst **c, t_lst *end, int nohang)
{
	t_lst		*cp;
	int			sym;

	cp = *c;
	while (cp != end)
	{
		sym = ((t_process *)cp->data)->c->endsym;
		job_wait_control((t_process *)cp->data,
						 (nohang ? WNOHANG : 0) | WUNTRACED);
		pop_lst__(&cp, NULL);
	}
	*c = cp;
	return (sym);
}

void	job_pipe_fork_(t_process *proc)
{
	int			fd[2];

	pipe(fd);
	dup2(fd[1], 1);
	close(fd[1]);
	proc->pid = fork();
	if (proc->pid == 0)
		job_exec_process(proc);
	dup2(fd[0], 0);
	close(fd[0]);
}

int		job_pipe_fork(t_lst **c, int nohang)
{
	int			fdsave[2];
	t_lst		*cp;
	t_process	*proc;

	cp = *c;
	fdsave[0] = dup(0);
	fdsave[1] = dup(1);
	proc = (t_process *)cp->data;
	while (proc->c->endsym == pip)
	{
		job_pipe_fork_(proc);
		cp = cp->next;
		proc = (t_process *)cp->data;
	}
	dup2(fdsave[1], 1);
	close(fdsave[1]);
	proc->pid = fork();
	if (proc->pid == 0)
		job_exec_process(proc);
	dup2(fdsave[0], 0);
	close(fdsave[0]);
	return (job_pipe_wait(c, cp->next, nohang));
}

void	job_cond_skip(t_lst **c, int sym)
{
	t_lst		*cp;
	t_process	*proc;

	cp = *c;
	while (cp)
	{
		proc = (t_process *)cp->data;
		if (proc->c->endsym != sym)
			break ;
		free_full_parsed_command(proc->c);
		pop_lst__(&cp, free);
	}
	free_full_parsed_command(proc->c);
	pop_lst__(&cp, free);
	*c = cp;
}

void	job_fork_alone(t_lst **c, int nohang)
{
	t_process	*proc;

	proc = (t_process *)(*c)->data;
	if (proc->c->cmd.type == C_SHELL_BUILTIN)
	{
		job_exec_process(proc);
		free_full_parsed_command(proc->c);
		pop_lst__(c, free);
	}
	else
	{
		proc->pid = fork();
		if (proc->pid == 0)
			job_exec_process(proc);
		job_wait_control(proc, (nohang ? WNOHANG : 0) | WUNTRACED);
		pop_lst__(c, NULL);
	}
}


void	job_cond_fork_(t_lst **c, int nohang)
{
	t_process	*proc;
	int			sym;
	t_lst		*cp;

	cp = *c;
	while (cp)
	{
		proc = (t_process *)cp->data;
		if ((sym = proc->c->endsym) == pip)
			sym = job_pipe_fork(&cp, 0);
		else
			job_fork_alone(&cp, 0);
		if (sym == andsym && g_laststatus)
			job_cond_skip(&cp, orsym);
		else if (sym == orsym && !g_laststatus)
			job_cond_skip(&cp, andsym);
	}
	*c = 0;
	if (nohang)
		exit(g_laststatus);
}

void	job_cond_fork(t_lst **c, int nohang)
{
	t_process	*proc;

	if (nohang)
	{
		if ((proc = malloc(sizeof(*proc))) != 0)
		{
			proc->flag = WNOHANG;
			proc->qid = g_jobs ? ((t_process *)g_jobs->data)->qid + 1 : 1;
			proc->c = (t_command *)0;
			proc->pid = fork();
			if (proc->pid == 0)
				job_cond_fork_(c, nohang);
			push_lst__(&g_jobs, proc);
		}
		while (*c)
			pop_lst__(c, free_full_parsed_command);
	}
	else
		job_cond_fork_(c, nohang);
}

void	job_exec(t_job *job)
{
	if (job->type & JTCOND)
		job_cond_fork(&job->proc, job->type & JTNOHANG);
	else if (job->type & JTPIPE)
		job_pipe_fork(&job->proc, job->type & JTNOHANG);
	else
		job_fork_alone(&job->proc, job->type & JTNOHANG);
	free(job);
}
