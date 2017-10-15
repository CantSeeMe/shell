/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   job9.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: root <marvin@42.fr>                        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2017/10/03 13:09:01 by root              #+#    #+#             */
/*   Updated: 2017/10/15 07:31:28 by jye              ###   ########.fr       */
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

void	job_abort_insert(t_process *proc)
{
	if (proc->state & JT_SUSPENDED)
	{
		kill(proc->pid, SIGKILL);
		waitpid(proc->pid, NULL, 0);
	}
	setpgid(proc->pid, proc->pid);
	ft_dprintf(2, "%s: Malloc error, process disowned: %d\n", "21sh", proc->pid);
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
	ft_dprintf(2, "%d\n", WSTOPSIG(proc->status));
	if (proc->state & JT_SUSPENDED)
		job_print_process_status(proc, g_js.jnodecur - 1, "suspended");
}

void	job_update_state(t_process *proc, int status)
{
	if (WIFSTOPPED(status) &&
		(WSTOPSIG(status) == SIGTTOU ||
		 WSTOPSIG(status) == SIGTTIN))
	{
		if (!(proc->state & JT_SUSPENDED))
		{
			g_js.suspended += 1;
			proc->state = JT_SUSPENDED;
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

void	job_check_jobs(void)
{
	int			i;
	int			status;
	t_process	*proc;

	i = 0;
	while (i < g_js.jnodecur)
	{
		if (g_jobs[i])
		{
			proc = g_jobs[i];
			if ((waitpid(proc->pid, &status, WNOHANG | WUNTRACED)))
			{
				job_update_state(proc, status);
				if (WIFSTOPPED(status))
					job_print_process_status(proc, i, g_sig_[WSTOPSIG(status)]);
				else
					job_print_process_status(proc, i, g_sig_[WTERMSIG(status)]);
			}
		}
		i++;
	}
	job_prune_dead_jobs();
}

void	job_signal_behavior(void (*behavior)(int))
{
	struct sigaction	act;

	sigfillset(&act.sa_mask);
	act.sa_handler = behavior;
	act.sa_flags = 0;
	sigaction(SIGINT, &act, 0);
	sigaction(SIGTSTP, &act, 0);
	sigaction(SIGTTOU, &act, 0);
	sigaction(SIGTTIN, &act, 0);
	sigaction(SIGTERM, &act, 0);
	sigaction(SIGTERM, &act, 0);
}

pid_t	job_make_child(int nohang)
{
	pid_t	pid;

	pid = fork();
	if (pid < 0)
		return (pid);
	if (pid == 0)
	{
		if (nohang)
			setpgid(0, 0);
		job_signal_behavior(SIG_DFL);
	}
	return (pid);
}

void	job_init_job_control(void)
{
	pid_t	pid;

	pid = getpgid(0);
	g_orgid = pid;
	pid = getpid();
	g_shgid = pid;
	setpgid(0, pid);
}
