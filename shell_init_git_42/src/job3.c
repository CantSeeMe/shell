/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   job3.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jye <jye@student.42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2017/09/21 20:39:15 by jye               #+#    #+#             */
/*   Updated: 2017/09/28 19:29:50 by jye              ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "lst.h"
#include "command.h"
#include "job.h"
#include "htcmd.h"
#include "htvar.h"
#include "token.h"
#include "etc_parse.h"

#include <stdlib.h>

static void		dummy_job_create(t_lst **command)
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

static void		job_test_malloc(t_job *job, t_lst **cp)
{
	void	*t;

	while (*cp && (t = malloc(sizeof(t_process))) == 0)
		pop_lst__(cp, free_half_parsed_command);
	free(t);
	job->proc = *cp;
}

static void		job_set_ready_for_exec(t_command *c)
{
	transmute_av(c);
	set_execpath(c);
	if ((c->cmd.type != C_SHELL_BUILTIN))
		vhash_set_underscore(c->cmd.c);
	c->envp = set_envp();
}

static t_lst	*job_create_process(t_job *job, t_lst *cp)
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
		job_set_ready_for_exec(c);
		p->pid = -1;
		job->type |= ((c->endsym == pip) * JTPIPE) |
					((c->endsym == andsym || c->endsym == orsym) * JTCOND) |
					((c->cmd.type == C_SHELL_BUILTIN) * JTBUILTIN);
		cp->data = p;
		p->c = c;
		if (c->endsym == ampersand || c->endsym == period)
			break ;
		cp = cp->next;
	}
	job->type |= c->endsym == ampersand ? JTNOHANG : JTHANG;
	return (cp ? cp->next : cp);
}

t_job			*job_create(t_lst **command)
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
