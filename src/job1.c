/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   job1.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jye <jye@student.42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2017/09/21 20:33:52 by jye               #+#    #+#             */
/*   Updated: 2017/11/11 00:42:03 by jye              ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "job.h"
#include "lst.h"
#include "token.h"
#include "ft_printf.h"

#include <stdlib.h>

static void	job_cond_skip(t_lst **c, int sym)
{
	t_lst		*cp;
	t_process	*proc;

	cp = *c;
	while (cp)
	{
		proc = (t_process *)cp->data;
		if (proc->c->endsym == sym)
			break ;
		free_full_parsed_command(proc->c);
		pop_lst__(&cp, free);
	}
	if (cp)
	{
		free_full_parsed_command(proc->c);
		pop_lst__(&cp, free);
	}
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
		if (JT_SIGNALED(g_js.pstat))
			ft_dprintf(2, "%s\n", g_sig_[JT_SIG(g_js.pstat)]);
		if (sym == andsym && (JT_SIGNALED(g_js.pstat) || JT_EXIT(g_js.pstat)))
			job_cond_skip(&cp, orsym);
		else if (sym == orsym &&
				!(JT_SIGNALED(g_js.pstat) || JT_EXIT(g_js.pstat)))
			job_cond_skip(&cp, andsym);
	}
	*c = 0;
	if (nohang)
		exit(JT_SIGNALED(g_js.pstat) ?
			JT_SIG(g_js.pstat) : JT_EXIT(g_js.pstat));
}

void		job_cond_fork(t_lst **c, int nohang)
{
	t_process	*proc;

	if (nohang)
	{
		if ((proc = malloc(sizeof(*proc))) != 0)
		{
			proc->flag = WNOHANG;
			proc->c = (t_command *)0;
			proc->pid = fork();
			if (proc->pid == 0)
				job_cond_fork_(c, nohang);
			job_insert(proc);
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
