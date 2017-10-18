/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   job12.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jye <jye@student.42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2017/10/16 03:39:45 by jye               #+#    #+#             */
/*   Updated: 2017/10/16 17:23:15 by root             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "job.h"
#include "ft_printf.h"

#include <unistd.h>
#include <stdlib.h>
#include <signal.h>
#include <sys/wait.h>

void	job_update_state(t_process *proc, int status)
{
	if (WIFSTOPPED(status) &&
		(WSTOPSIG(status) == SIGTTOU ||
		WSTOPSIG(status) == SIGTTIN))
	{
		if (!(proc->state & JT_SUSPENDED))
		{
			g_js.suspended += 1;
			proc->state |= JT_SUSPENDED;
			proc->status = status;
		}
	}
	else
	{
		if (proc->state & JT_SUSPENDED)
			g_js.suspended -= 1;
		proc->state = JT_DEAD;
	}
	proc->status = status;
}

void	job_prune_dead_jobs(void)
{
	int			i;
	t_process	*proc;

	i = 0;
	while (i < g_js.jnodecur)
	{
		proc = g_jobs[i];
		if (proc && proc->state & JT_DEAD)
		{
			g_jobs[i] = (t_process *)0;
			free_full_parsed_command(proc->c);
			free(proc);
		}
		i++;
	}
}

void	job_reset_ind(void)
{
	if (g_js.jnodecur == 0)
		return ;
	if (g_jobs[g_js.cur] == (t_process *)0)
		g_js.cur = g_js.prev;
	while (g_js.prev >= 0)
	{
		if (g_jobs[g_js.prev])
			break ;
		g_js.prev--;
	}
	while (g_js.jnodecur)
	{
		if (g_jobs[g_js.jnodecur - 1])
			break ;
		g_js.jnodecur--;
	}
	if (g_js.prev == -1 && g_js.jnodecur - 1 != g_js.cur)
		g_js.prev = g_js.jnodecur - 1;
}

void	job_check_jobs(void)
{
	int			i;
	int			status;
	t_process	*proc;
	int			ll;

	i = 0;
	ll = 0;
	while (i < g_js.jnodecur)
	{
		if ((proc = g_jobs[i]) && !(proc->state & JT_DEAD))
			if ((waitpid(proc->pid, &status, WNOHANG | WUNTRACED)) > 0)
			{
				if (!ll)
				{
					ll = 1;
					ft_putchar('\n');
				}
				job_update_state(proc, status);
				job_print_process_status(proc, i, g_sig_[WIFSTOPPED(status) ?
										WSTOPSIG(status) : WTERMSIG(status)]);
			}
		i++;
	}
	job_prune_dead_jobs();
	job_reset_ind();
}
