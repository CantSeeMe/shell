/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   bg.c                                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jye <jye@student.42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2017/10/15 18:07:35 by jye               #+#    #+#             */
/*   Updated: 2017/10/16 07:33:10 by jye              ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "job.h"
#include "libft.h"
#include "ft_printf.h"
#include "job_target.h"

#include <sys/wait.h>
#include <unistd.h>
#include <stdlib.h>
#include <signal.h>

void	bg_2(t_process *p, int status, int tar)
{
	if (WIFSTOPPED(status))
	{
		if (!(p->state & JT_SUSPENDED))
			g_js.suspended += 1;
		p->state |= JT_SUSPENDED;
		job_print_process_status(p, tar, g_sig_[WSTOPSIG(status)]);
	}
	else if (WIFEXITED(status) || WIFSIGNALED(status))
	{
		if (p->state & JT_SUSPENDED)
			g_js.suspended -= 1;
		p->state = JT_DEAD;
		if (WIFSIGNALED(status))
			job_print_process_status(p, tar, g_sig_[WTERMSIG(status)]);
	}
}

int		ft_bg(int ac, char **av, char **envp)
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
	if (p->state & JT_SUSPENDED)
		kill(p->pid, SIGCONT);
	status = job_wait_control_(p->pid, WUNTRACED | WNOHANG);
	if (JT_IS_NOHANG(status))
		return (0);
	p->status = status;
	return (0);
}
