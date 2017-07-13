/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   job.c                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jye <jye@student.42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2017/07/13 16:07:52 by jye               #+#    #+#             */
/*   Updated: 2017/07/13 18:30:31 by jye              ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <stdlib.h>
#include <unistd.h>
#include <stdio.h>
#include <signal.h>
#include <errno.h>
#include <string.h>

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

	sigemptyset(&set); // initialize sigset
	sigaddset(&set, SIGTSTP); // adding some mask
	sigaddset(&set, SIGINT);

	id = fork();
	if (id == 0) /* child */
	{
//		sigprocmask(SIG_UNBLOCK, &set, NULL); // unblock signal on current process
		return (execve("/bin/ls", smth, envp));
	}
	else
	{
		/* father */
		sigprocmask(SIG_BLOCK, &set, NULL); // block signal on current process
		waitpid(id, &status, WUNTRACED);
		dprintf(2, "%d %d %d %d %s\n", WEXITSTATUS(status), WTERMSIG(status),
				WCOREDUMP(status), WSTOPSIG(status), strerror(errno));
		if (WIFSTOPPED(status))
		{
			kill(id, SIGCONT);
		}
	}
	waitpid(id, &status, 0);
	dprintf(2, "%d %d %s\n", WEXITSTATUS(status), id, strerror(errno));
	/* WNOHANG for no pchild process wait */
}
