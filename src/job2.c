/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   job2.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jye <jye@student.42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2017/09/21 20:35:52 by jye               #+#    #+#             */
/*   Updated: 2017/11/14 08:20:44 by jye              ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "lst.h"
#include "command.h"
#include "job.h"
#include "token.h"
#include "ft_printf.h"

#include <unistd.h>
#include <fcntl.h>
#include <sys/wait.h>

int		job_pipe_wait(t_lst **c, t_lst *end, int nohang)
{
	t_lst		*cp;
	t_process	*p;
	int			sym;

	cp = *c;
	while (cp != end)
	{
		p = (t_process *)cp->data;
		sym = p->c->endsym;
		job_wait_control(p, (nohang ? WNOHANG : 0) | WUNTRACED);
		pop_lst__(&cp, 0);
	}
	if (JT_SIGNALED(g_js.pstat))
		ft_dprintf(2, "%s\n", g_sig_[JT_SIG(g_js.pstat)]);
	*c = cp;
	return (sym);
}

pid_t	job_pipe_fork_(t_process *proc, int nohang, pid_t pgid)
{
	int			fd[2];

	pipe(fd);
	dup2(fd[1], 1);
	close(fd[1]);
	proc->pid = job_make_child(nohang, pgid);
	if (proc->pid == 0)
	{
		close(fd[0]);
		job_exec_process(proc);
	}
	dup2(fd[0], 0);
	close(fd[0]);
	return (pgid ? pgid : proc->pid);
}

int		job_pipe_fork(t_lst **c, int nohang)
{
	int			fdsave[2];
	t_lst		*cp;
	t_process	*proc;
	pid_t		pid;

	cp = *c;
	fdsave[0] = dup(0);
	fdsave[1] = dup(1);
	proc = (t_process *)cp->data;
	pid = 0;
	while (proc->c->endsym == pip)
	{
		pid = job_pipe_fork_(proc, nohang, pid);
		cp = cp->next;
		proc = (t_process *)cp->data;
	}
	dup2(fdsave[1], 1);
	close(fdsave[1]);
	proc->pid = job_make_child(nohang, pid);
	if (proc->pid == 0)
		job_exec_process(proc);
	dup2(fdsave[0], 0);
	close(fdsave[0]);
	return (job_pipe_wait(c, cp->next, nohang));
}
