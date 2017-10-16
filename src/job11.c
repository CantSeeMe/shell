/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   job11.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jye <jye@student.42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2017/10/16 03:38:29 by jye               #+#    #+#             */
/*   Updated: 2017/10/16 03:47:39 by jye              ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "job.h"
#include "ft_printf.h"

#include <unistd.h>
#include <stdlib.h>
#include <signal.h>
#include <sys/wait.h>

int		job_realloc_joblist(size_t s)
{
	t_process	**cp;
	int			i;

	cp = g_jobs;
	if ((g_jobs = malloc(sizeof(*g_jobs) * s)) == 0)
	{
		g_jobs = cp;
		return (1);
	}
	i = 0;
	while (i < g_js.jnodesize)
	{
		g_jobs[i] = cp[i];
		++i;
	}
	g_js.jnodesize = s;
	while (i < g_js.jnodesize)
		g_jobs[i++] = 0;
	return (0);
}

void	job_abort_insert(t_process *proc)
{
	if (proc->state & JT_SUSPENDED)
	{
		kill(proc->pid, SIGKILL);
		waitpid(proc->pid, NULL, 0);
	}
	setpgid(proc->pid, proc->pid);
	free_full_parsed_command(proc->c);
	free(proc);
}

void	job_insert_(t_process *proc)
{
	g_jobs[g_js.jnodecur] = proc;
	if (proc->state & JT_SUSPENDED)
	{
		g_js.prev = g_js.cur;
		g_js.cur = g_js.jnodecur;
		g_js.suspended += 1;
	}
	else
	{
		if (g_js.cur == -1)
			g_js.cur = g_js.jnodecur;
		else
			g_js.prev = g_js.jnodecur;
	}
	g_js.jnodecur++;
}

void	job_insert(t_process *proc)
{
	if (g_js.jnodecur == g_js.jnodesize)
		if (job_realloc_joblist(g_js.jnodesize + JT_DEFAULT_SIZE))
			job_abort_insert(proc);
	job_insert_(proc);
	if (proc->state & JT_SUSPENDED)
		job_print_process_status(proc, g_js.jnodecur - 1, "suspended");
	else if (proc->state & JT_BACKGROUND)
		ft_dprintf(1, "[%d] %d\n", g_js.jnodecur, proc->pid);
}
