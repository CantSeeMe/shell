/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exit_.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: root <marvin@42.fr>                        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2017/09/14 16:37:33 by root              #+#    #+#             */
/*   Updated: 2017/09/24 15:26:36 by jye              ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "job.h"
#include "lst.h"
#include "ft_printf.h"

#include <signal.h>
#include <sys/wait.h>
#include <sys/types.h>
#include <stdlib.h>

void	ft_exit_kill_job(void)
{
	t_process	*proc;
	int			status;

	while (g_jobs)
	{
		proc = g_jobs->data;
		waitpid(proc->pid, &status, WUNTRACED | WNOHANG);
		if (proc->flag & ~(0xffff))
			kill(proc->pid, SIGKILL);
		g_jobs = g_jobs->next;
	}
}

int		ft_exitsh(int ac, char **av, char **envp)
{
	(void)ac;
	(void)av;
	(void)envp;
	if (g_jobs)
	{
		if ((g_laststatus & ~(0xffff)))
			ft_exit_kill_job();
		else
		{
			ft_dprintf(1, "%s: there are suspended jobs\n", "21sh");
			g_laststatus |= ~(0xffff);
			return (1);
		}
	}
	exit(g_laststatus & 0xff);
	return (0);
}
