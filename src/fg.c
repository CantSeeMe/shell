/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   fg.c                                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jye <jye@student.42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2017/10/08 23:46:32 by jye               #+#    #+#             */
/*   Updated: 2017/10/16 03:53:06 by jye              ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "fg.h"
#include "job.h"
#include "libft.h"
#include "ft_printf.h"
#include "job_target.h"

#include <sys/wait.h>
#include <unistd.h>
#include <stdlib.h>
#include <signal.h>

void	fg_2(t_process *p, int status, int tar)
{
	if (WIFSTOPPED(status))
	{
		if (!(p->state & JT_SUSPENDED))
			g_js.suspended += 1;
		p->state |= JT_SUSPENDED;
		job_print_process_status(p, tar, g_sig_[WSTOPSIG(status)]);
	}
	else
	{
		if (p->state & JT_SUSPENDED)
			g_js.suspended -= 1;
		p->state = JT_DEAD;
		job_print_process_status(p, tar, g_sig_[WTERMSIG(status)]);
	}
}

int		ft_fg(int ac, char **av, char **envp)
{
	int			tar;
	t_process	*p;
	int			status;

	(void)ac;
	(void)envp;
	tar = job_get_target_job(av);
	if (tar == -1)
	{
		if (av[1])
			ft_dprintf(2, "%s: fg: %s job not found\n", "21sh", av[1]);
		else
			ft_dprintf(2, "%s: fg: no current job\n", "21sh");
		return (1);
	}
	p = g_jobs[tar];
	job_print_process_status(p, tar, g_sig_[SIGCONT]);
	tcsetpgrp(2, p->pid);
	if (p->state & JT_SUSPENDED)
		kill(p->pid, SIGCONT);
	status = job_wait_control_(p->pid, WUNTRACED);
	p->status = status;
	fg_2(p, status, tar);
	return (0);
}
