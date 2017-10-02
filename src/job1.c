/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   job1.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jye <jye@student.42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2017/09/21 20:33:52 by jye               #+#    #+#             */
/*   Updated: 2017/09/25 13:42:03 by root             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "job.h"
#include "lst.h"
#include "token.h"

#include <stdlib.h>

static void	job_cond_skip(t_lst **c, int sym)
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

static void	job_cond_fork_(t_lst **c, int nohang)
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
		if (sym == andsym && (g_laststatus & 0xff))
			job_cond_skip(&cp, orsym);
		else if (sym == orsym && !(g_laststatus & 0xff))
			job_cond_skip(&cp, andsym);
	}
	*c = 0;
	if (nohang)
		exit(g_laststatus & 0xff);
}

void		job_cond_fork(t_lst **c, int nohang)
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
		{
			proc = (t_process *)(*c)->data;
			free_full_parsed_command(proc->c);
			pop_lst__(c, free);
		}
	}
	else
		job_cond_fork_(c, nohang);
}
