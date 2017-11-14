/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   job7.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jye <jye@student.42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2017/09/21 21:02:28 by jye               #+#    #+#             */
/*   Updated: 2017/11/14 08:21:09 by jye              ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "command.h"
#include "error.h"
#include "job.h"
#include "htcmd.h"
#include "htvar.h"
#include "exit_.h"
#include "ft_printf.h"

#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h>

void	job_fork_exec(t_command *c)
{
	int	i;

	if (c->av.cav[c->var_] == 0 || *c->av.cav[c->var_] == 0)
		exit(0);
	if ((i = test_execpath(c->cmd.c)))
	{
		exec_error("21sh", *(c->av.cav + c->var_), i - 1);
		exit(126 + (i <= 2));
	}
	exit(execve((const char *)c->cmd.c, c->av.cav + c->var_, c->envp));
}

void	job_builtin_exec(t_process *p)
{
	t_command	*c;

	c = p->c;
	if (p->pid == 0)
		exit(((t_builtin)c->cmd.c)
			(c->ac - c->var_, c->av.cav + c->var_, c->envp));
	g_js.pstat = (((t_builtin)c->cmd.c)
				(c->ac - c->var_, c->av.cav + c->var_, c->envp)) << 8;
	g_js.exit = c->cmd.c == ft_exitsh;
}

int		job_exec_process(t_process *p)
{
	t_lst		*rest;

	if ((rest = job_openfd(p->c->redir)) == (t_lst *)-1)
	{
		if (p->pid == 0)
			exit(127);
		else
			return (127 << 8);
	}
	p->c->redir = (t_lst *)0;
	if (p->c->cmd.type == C_SHELL_BUILTIN)
	{
		job_builtin_exec(p);
		job_restorefd(rest);
		return (g_js.pstat);
	}
	job_fork_exec(p->c);
	return (127 << 8);
}

void	job_fork_alone(t_lst **c, int nohang)
{
	t_process	*proc;

	proc = (t_process *)(*c)->data;
	vhash_set_underscore(*(proc->c->av.cav + proc->c->ac - 1));
	if (proc->c->cmd.type == C_SHELL_BUILTIN)
	{
		job_exec_process(proc);
		free_full_parsed_command(proc->c);
		pop_lst__(c, free);
	}
	else
	{
		proc->pid = job_make_child(nohang, 0);
		if (proc->pid == 0)
			job_exec_process(proc);
		job_wait_control(proc, (nohang ? WNOHANG : 0) | WUNTRACED);
		pop_lst__(c, 0);
	}
}
