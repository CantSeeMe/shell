/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   job.c                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jye <jye@student.42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2017/07/13 16:07:52 by jye               #+#    #+#             */
/*   Updated: 2017/07/13 17:25:15 by jye              ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <stdlib.h>
#include <unistd.h>
#include <stdio.h>
#include <signal.h>
#include <errno.h>
#include <string.h>
#include "lst.h"

typedef struct	s_job
{
	pid_t	pid;
	int		status;
}				t_job;

int		main(int ac, char **av, char **envp)
{
	pid_t	id;
	char	*smth[] = {"ls", "-R", getenv("HOME"), NULL};
	int		status;
	sigset_t	set;
	sigset_t	old;

	sigfillset(&set);
	sigdelset(&set, SIGSTOP);
	sigdelset(&set, SIGINT);

	sigprocmask(SIG_BLOCK, &set, NULL);
	id = fork();
	if (id == 0) /* child */
	{
		sigprocmask(SIG_UNBLOCK, &set, NULL);
		execve("/bin/ls", smth, envp);
		return (0);
	}
	else
	{
		/* father */
//		kill(id, SIGSTOP);
		waitpid(id, &status, WUNTRACED);
		dprintf(2, "%d %d %s\n", WEXITSTATUS(status), id, strerror(errno));
		if (WIFSTOPPED(status))
		{
			kill(id, SIGCONT);
		}
	}
	waitpid(id, &status, WUNTRACED);
	dprintf(2, "%d %d %s\n", WEXITSTATUS(status), id, strerror(errno));
}
