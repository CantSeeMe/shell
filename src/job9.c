/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   job9.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: root <marvin@42.fr>                        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2017/10/03 13:09:01 by root              #+#    #+#             */
/*   Updated: 2017/10/09 08:36:56 by jye              ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "job.h"
#include "ft_printf.h"

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
		g_js.cur = g_js.jnodecur;
	else
	{
		if (g_jobs[g_js.jnodecur - 1]->state & JT_SUSPENDED)
		{
			g_js.prev = g_js.cur;
			g_js.cur = g_js.jnodecur;
		}
		else
		{
			g_js.prev = g_js.jnodecur;
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
				waitpid(proc->pid, NULL, 0);
			}
			free_full_parsed_command(proc->c);
			free(proc);
			return ;
		}
	}
	g_jobs[g_js.jnodecur] = proc;
	job_set_order();
	ft_dprintf(1, "[%d] %s%s %d %s\n", g_js.jnodecur + 1,
			   g_js.jnodecur == g_js.prev ? "-" : "",
			   g_js.jnodecur == g_js.cur ? "+" : "",
			   g_jobs[g_js.jnodecur]->pid,
			   g_jobs[g_js.jnodecur]->state & JT_SUSPENDED ? "suspended"
			   : "");
	if (g_jobs[g_js.jnodecur++]->state & JT_SUSPENDED)
		g_js.suspended += 1;
}

void	job_print_process_status(t_process *proc, int status, int qid)
{
	static const char	*sig_[] = {"Exited", "Hangup", "Interrupt", "Quit",
								   "Illegal Instruction", "Trapped",
								   "Aborted", "EMT instruction",
								   "Floating point exception", "Killed",
								   "Bus error", "Segmentation fault",
								   "Bad argument", "Broken pipe",
								   "Alarm clock", "Terminated",
								   "err idk", "Stopped", "Stopped",
								   "err idk",
								   "Continued", "Stopped", "Stopped",
								   "err idk", "CPU limit", "File limit"};

	ft_dprintf(1, "[%d] %s %d %s\n", qid, proc->c ? *(proc->c->av.cav + proc->c->var_) : "", proc->pid,
			   sig_[WTERMSIG(status)]);
}

void	job_check_list(void)
{
	int	i;
	int	status;

	i = 0;
	while (i < g_js.jnodecur)
	{
		if (g_jobs[i] &&
			waitpid(g_jobs[i]->pid, &status, WNOHANG) > 0)
		{
			if (JT_TERMINATED(status))
			{
				if ((g_jobs[i]->state & JT_SUSPENDED))
					g_js.suspended -= 1;
				job_print_process_status(g_jobs[i], status, i + 1);
				free(g_jobs[i]);
				g_jobs[i] = (t_process *)0;
			}
		}
		i++;
	}
	while (g_js.jnodecur)
	{
		if (g_jobs[g_js.jnodecur - 1])
			break ;
		g_js.jnodecur--;
	}
}
