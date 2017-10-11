/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   job9.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: root <marvin@42.fr>                        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2017/10/03 13:09:01 by root              #+#    #+#             */
/*   Updated: 2017/10/11 13:06:01 by root             ###   ########.fr       */
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
