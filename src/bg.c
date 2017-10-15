/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   bg.c                                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jye <jye@student.42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2017/10/15 18:07:35 by jye               #+#    #+#             */
/*   Updated: 2017/10/15 18:21:04 by jye              ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "job.h"
#include "libft.h"
#include "ft_printf.h"

#include <sys/wait.h>
#include <unistd.h>
#include <stdlib.h>
#include <signal.h>

int		bg_test_target(char *s)
{
	int	t;

	t = ft_atoi(s);
	if (t >= g_js.jnodecur)
		return (1);
	else if (g_jobs[t])
	{
		g_js.prev = g_js.cur;
		g_js.cur = t;
		return (0);
	}
	return (1);
}

int		bg_get_target_job(char **av)
{
	if (g_js.cur == -1)
		return (-1);
	else if (av[1] == NULL)
		return (g_js.cur);
	else if (av[1][0] == '%')
	{
		if (av[1][1] <= 0x39 && av[1][1] >= 0x30)
			return (bg_test_target(av[1] + 1) ?
					-1 : g_js.cur);
		else if (av[1][1] == '+')
			return (g_js.cur);
		else if (av[1][1] == '-' && g_js.prev != -1)
		{
			g_js.prev ^= g_js.cur;
			g_js.cur ^= g_js.prev;
			g_js.prev ^= g_js.cur;
			return (g_js.cur);
		}
	}
	return (-1);
}

int		ft_bg(int ac, char **av, char **envp)
{
	int			tar;
	t_process	*p;
	int			status;

	(void)ac;
	(void)envp;
	tar = bg_get_target_job(av);
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
	return (0);
}
