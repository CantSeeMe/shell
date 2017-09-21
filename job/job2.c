/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   job2.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jye <jye@student.42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2017/09/21 20:35:52 by jye               #+#    #+#             */
/*   Updated: 2017/09/21 21:09:54 by jye              ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "lst.h"
#include "command.h"
#include "job.h"

#include <unistd.h>
#include <fcntl.h>

int		job_pipe_wait(t_lst **c, t_lst *end, int nohang)
{
	t_lst		*cp;
	int			sym;

	cp = *c;
	while (cp != end)
	{
		sym = ((t_process *)cp->data)->c->endsym;
		job_wait_control((t_process *)cp->data,
							(nohang ? WNOHANG : 0) | WUNTRACED);
		pop_lst__(&cp, 0);
	}
	*c = cp;
	return (sym);
}

void	job_pipe_fork_(t_process *proc)
{
	int			fd[2];

	pipe(fd);
	dup2(fd[1], 1);
	close(fd[1]);
	proc->pid = fork();
	if (proc->pid == 0)
		job_exec_process(proc);
	dup2(fd[0], 0);
	close(fd[0]);
}

int		job_pipe_fork(t_lst **c, int nohang)
{
	int			fdsave[2];
	t_lst		*cp;
	t_process	*proc;

	cp = *c;
	fdsave[0] = dup(0);
	fdsave[1] = dup(1);
	proc = (t_process *)cp->data;
	while (proc->c->endsym == pip)
	{
		job_pipe_fork_(proc);
		cp = cp->next;
		proc = (t_process *)cp->data;
	}
	dup2(fdsave[1], 1);
	close(fdsave[1]);
	proc->pid = fork();
	if (proc->pid == 0)
		job_exec_process(proc);
	dup2(fdsave[0], 0);
	close(fdsave[0]);
	return (job_pipe_wait(c, cp->next, nohang));
}
