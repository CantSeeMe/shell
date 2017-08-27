/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   base.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jye <jye@student.42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2017/08/16 04:00:33 by jye               #+#    #+#             */
/*   Updated: 2017/08/27 02:38:10 by jye              ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "hashlib.h"
#include "ft_readline.h"
#include "token.h"
#include "command.h"
#include "lst.h"
#include "htcmd.h"
#include "htvar.h"
#include "error.h"
#include "job.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <signal.h>
#include <unistd.h>


int		transmute_av(t_command *c)
{
	char	**av;
	t_lst	*z;
	int		i;

	if ((av = malloc(sizeof(*av) * (c->ac + 1))) == 0)
		return (1);
	z = c->av.lav;
	i = c->ac;
	av[i--] = 0;
	while (z)
	{
		av[i--] = z->data;
		pop_lst__(&z, 0);
	}
	c->av.cav = av;
	return (0);
}

/******************************/
/******************************/
/******************************/

int		set_execpath(t_command *c)
{
	t_ccsh		*z;
	t_var		*v;

	if (strchr(*c->av.cav, '='))
	{
		if ((v = init_var(*c->av.cav, 0)))
			vhash_insert(v);
		c->var_ = 1;
	}
	else
	{
		c->var_ = 0;
	}
	c->cmd.type = C_SHELL_EXT;
	if (strchr(*c->av.cav + c->var_, '/'))
		c->cmd.c = (*c->av.cav + c->var_);
	else if ((z = chash_lookup(*(c->av.cav + c->var_), vhash_search("PATH"))))
		c->cmd = *z;
	else
		c->cmd.c = 0;
//	dprintf(2, "%s\n", c->cmd.c);
	return (0);
}

int		set_envp(t_command *c)
{
	int		i;
	t_lst	*envp;
	t_var	*v;

	if ((c->envp = malloc(sizeof(char *) * (g_envpsize + 1))) == 0)
		return (1);
	i = g_envpsize;
	c->envp[i--] = 0;
	envp = g_envp;
	while (envp)
	{
		v = envp->data;
		c->envp[i--] = defrag_var(v);
		envp = envp->next;
	}
	return (0);
}

int		main(int ac, char **av, char **envp)
{
	char	*s;
	t_lst	*t;
	t_command	*c;

	init_htvar(envp);
	chash_init();
	
	s = ft_readline("minishell> ", strlen("minishell> "));
	init_tokenizer();
	t = tokenize(s);
	t = parse_token(t);
	c = t->data;
//	dprintf(1, "%s\n", c->av.lav->data);
/////
	t_lst	*cp = t;
	while (cp)
	{
		c = cp->data;
		transmute_av(c);
		set_execpath(c);
		set_envp(c);
		cp = cp->next;
	}
	while (t)
	{
		t_job	*job;
		job = job_create(&t);
		job_exec(job);
	}
	// cut the pack of command into jobs of ; / &
	/////
/*****process parsed bullshit in a fork or not******/
}
