/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   fg.c                                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jye <jye@student.42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2017/10/08 23:46:32 by jye               #+#    #+#             */
/*   Updated: 2017/10/09 15:35:41 by root             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "fg.h"
#include "job.h"
#include "libft.h"
#include "ft_printf.h"

#include <sys/wait.h>
#include <unistd.h>
#include <stdlib.h>
#include <signal.h>

int		fg_test_target(char *s)
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

int		fg_get_target_job(char **av)
{
	if (g_js.cur == -1)
		return (-1);
	else if (av[1] == NULL)
		return (g_js.cur);
	else if (av[1][0] == '%')
	{
		if (av[1][1] <= 0x39 && av[1][1] >= 0x30)
			return (fg_test_target(av[1] + 1) ?
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

int		ft_fg(int ac, char **av, char **envp)
{
	int			tar;
	t_process	*p;
	int			status;

	(void)ac;
	(void)envp;
	tar = fg_get_target_job(av);
	if (tar == -1)
	{
		if (av[1])
			ft_dprintf(2, "%s: fg: %s job not found\n", "21sh", av[1]);
		else
			ft_dprintf(2, "%s: fg: no current job\n", "21sh");
		return (1);
	}
	p = g_jobs[tar];
	if (p->state & JT_SUSPENDED)
		kill(p->pid, SIGCONT);
	status = job_wait_control_(p->pid, WUNTRACED);
	if ((WIFSIGNALED(status) && !WIFSTOPPED(status))
		|| WIFEXITED(status))
	{
		if (p->state & JT_SUSPENDED)
			g_js.suspended -= 1;
		free_full_parsed_command(p->c);
		free(p);
		g_jobs[tar] = (t_process *)0;
		while (g_js.jnodecur)
		{
			if (g_jobs[g_js.jnodecur - 1])
				break ;
			g_js.jnodecur--;
		}
		g_js.cur = g_js.prev;
		while (g_js.prev >= 0)
		{
			if (g_jobs[g_js.prev - 1])
				break ;
			g_js.prev--;
		}
		if (g_js.prev == -1)
			g_js.prev = g_js.jnodecur - 1;
	}
	return (0);
}
