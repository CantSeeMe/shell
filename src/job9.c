/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   job9.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: root <marvin@42.fr>                        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2017/10/03 13:09:01 by root              #+#    #+#             */
/*   Updated: 2017/10/04 20:59:50 by root             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "job.h"
#include <unistd.h>
#include <stdlib.h>
#include <signal.h>

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

void	job_insert_to_list(t_process *proc)
{
	if (g_js.jnodecur == g_js.jnodesize)
	{
		if (job_realloc_joblist(g_js.jnodesize + JT_DEFAULT_SIZE))
		{
			if (proc->state & JT_SUSPENDED)
			{
				kill(proc->pid, SIGKILL);
				waitpid()
			}
		}
	}
}
