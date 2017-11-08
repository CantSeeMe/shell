/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   job9.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: root <marvin@42.fr>                        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2017/10/03 13:09:01 by root              #+#    #+#             */
/*   Updated: 2017/11/08 20:35:58 by jye              ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "job.h"
#include "ft_printf.h"

#include <unistd.h>
#include <stdlib.h>
#include <signal.h>
#include <sys/wait.h>

void	job_signal_behavior(void (*behavior)(int))
{
	struct sigaction	act;

	sigfillset(&act.sa_mask);
	act.sa_handler = behavior;
	act.sa_flags = 0;
	sigaction(SIGINT, &act, 0);
	sigaction(SIGTSTP, &act, 0);
	sigaction(SIGTTOU, &act, 0);
	sigaction(SIGTTIN, &act, 0);
	sigaction(SIGQUIT, &act, 0);
	sigaction(SIGTERM, &act, 0);
}

pid_t	job_make_child(int nohang)
{
	pid_t	pid;

	pid = fork();
	if (pid < 0)
		return (pid);
	if (pid == 0)
	{
		job_signal_behavior(SIG_DFL);
	}
	else
	{
		setpgid(pid, pid);
		if (!nohang)
			tcsetpgrp(2, pid);
	}
	return (pid);
}

int		job_init_job_control(void)
{
	pid_t	pid;

	pid = getpgid(0);
	g_orgid = pid;
	pid = getpid();
	g_shgid = pid;
	return (setpgid(0, pid));
}
