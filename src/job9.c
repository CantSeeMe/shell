/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   job9.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: root <marvin@42.fr>                        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2017/10/03 13:09:01 by root              #+#    #+#             */
/*   Updated: 2017/10/07 12:29:09 by root             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "job.h"

#include <unistd.h>
#include <stdlib.h>
#include <signal.h>
#include <sys/wait.h>

#define JT_TERMINATED(x) (WIFSIGNALED((x)) || WIFEXITED((x)))

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

void	job_set_order(void)
{
	if (g_js.cur == -1)
		g_js.cur = g_js.jnodecur - 1;
	else
	{
		if (g_jobs[g_js.jnodecur - 1].state & JT_SUSPENDED)
		{
			g_js.prev = g_js.cur;
			g_js.cur = g_js.jnodecur - 1;
		}
		else
		{
			g_js.prev = g_js.jnodecur - 1;
		}
	}
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
				waitpid(proc->pid);
			}
			free_full_parsed_command(proc->c);
			free(proc);
			return ;
		}
	}
	g_jobs[g_js.jnodecur++] = proc;
	job_set_order();
}

void	job_check_list(void)
{
	int	i;
	int	status;

	i = 0;
	while (i < g_js.jnodecur)
	{
		if (waipid(g_jobs[i]->pid, &status, WNOHANG) > 0)
		{
			if (JT_TERMINATED(status))
			{
				g_jobs[i]->state = JT_DEAD;
			}
		}
		i++;
	}
}
