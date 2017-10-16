/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   job_.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jye <jye@student.42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2017/10/15 21:10:38 by jye               #+#    #+#             */
/*   Updated: 2017/10/15 21:17:44 by jye              ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "job_.h"
#include "job.h"
#include "libft.h"
#include "ft_printf.h"

#include <sys/wait.h>
#include <unistd.h>
#include <stdlib.h>
#include <signal.h>

int		ft_job(int ac, char **av, char **envp)
{
	int			i;
	t_process	*p;

	i = 0;
	(void)ac;
	(void)av;
	(void)envp;
	while (i < g_js.jnodecur)
	{
		if ((p = g_jobs[i]))
		{
			if (WIFSTOPPED(p->status))
				job_print_process_status(p, i, g_sig_[WSTOPSIG(p->status)]);
			else if (WIFSIGNALED(p->status))
				job_print_process_status(p, i, g_sig_[WTERMSIG(p->status)]);
		}
		i++;
	}
	return (0);
}
