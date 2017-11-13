/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exit_.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: root <marvin@42.fr>                        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2017/09/14 16:37:33 by root              #+#    #+#             */
/*   Updated: 2017/11/13 20:49:51 by jye              ###   ########.fr       */
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
	int			i;
	int			status;

	i = 0;
	while (i < g_js.jnodecur)
	{
		if ((proc = g_jobs[i]))
		{
			if (proc->state & JT_SUSPENDED)
			{
				kill(proc->pid, SIGKILL);
				waitpid(proc->pid, &status, WNOHANG);
			}
		}
		i++;
	}
}

int		ft_exitsh(int ac, char **av, char **envp)
{
	(void)ac;
	(void)av;
	(void)envp;
	if (g_js.suspended)
	{
		if (g_js.exit)
			ft_exit_kill_job();
		else
		{
			ft_dprintf(1, "%s: there are suspended jobs\n", "21sh");
			g_js.exit = 1;
			return (1);
		}
	}
	exit((JT_SIGNALED(g_js.pstat) * (JT_SIG(g_js.pstat) + 128)) |
		JT_EXIT(g_js.pstat));
	return (0);
}
