/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   job7.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jye <jye@student.42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2017/09/21 21:02:28 by jye               #+#    #+#             */
/*   Updated: 2017/09/24 16:10:49 by jye              ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "command.h"
#include "error.h"
#include "job.h"
#include "htcmd.h"
#include "ft_printf.h"

#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h>

void	job_fork_exec(t_command *c)
{
	int	i;

	if (c->var_ && c->av.cav[1] == 0)
		exit(0);
	if ((i = test_execpath(c->cmd.c)))
	{
		exec_error("minishell", *(c->av.cav + c->var_), i - 1);
		exit(126 + (i <= 2));
	}
	exit(execve((const char *)c->cmd.c, c->av.cav + c->var_, c->envp));
}

int		job_exec_process(t_process *p)
{
	t_command	*c;
	t_lst		*rest;
	int			i;

	c = p->c;
	i = c->var_;
	rest = job_openfd(p->c->redir);
	if (c->cmd.type == C_SHELL_BUILTIN)
	{
		if (p->pid == 0)
			exit(((t_builtin)c->cmd.c)(c->ac - c->var_,
										c->av.cav + c->var_,
										c->envp));
		g_laststatus &= ~0xffff;
		g_laststatus |= (((t_builtin)c->cmd.c)
						(c->ac - c->var_,
						c->av.cav + c->var_,
						c->envp));
		job_restorefd(rest);
		return (g_laststatus);
	}
	job_fork_exec(c);
	return (127 << 8);
}

void	job_fork_alone(t_lst **c, int nohang)
{
	t_process	*proc;

	proc = (t_process *)(*c)->data;
	if (proc->c->cmd.type == C_SHELL_BUILTIN)
	{
		job_exec_process(proc);
		free_full_parsed_command(proc->c);
		pop_lst__(c, free);
	}
	else
	{
		proc->pid = fork();
		if (proc->pid == 0)
			job_exec_process(proc);
		job_wait_control(proc, (nohang ? WNOHANG : 0) | WUNTRACED);
		pop_lst__(c, 0);
	}
}
