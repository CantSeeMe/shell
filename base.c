/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   base.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jye <jye@student.42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2017/08/16 04:00:33 by jye               #+#    #+#             */
/*   Updated: 2017/08/23 14:28:10 by root             ###   ########.fr       */
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

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <sys/stat.h>
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

int		test_execpath(char *c)
{
	struct stat stats;

	if (c == 0)
		return (0);
	if (!strchr(c, '/'))
	{
		exec_error(c, 0);
		return (1);
	}
	if (stat(c, &stats))
	{
		exec_error(c, 1);
		return (1);
	}
	if (S_ISDIR(stats.st_mode))
	{
		exec_error(c, 2);
		return (1);
	}
	if (!(stats.st_mode & (S_IXUSR | S_IXGRP | S_IXOTH)))
	{
		exec_error(c, 3);
		return (1);
	}
	return (0);
}

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
	if ((z = chash_lookup(*(c->av.cav + c->var_), vhash_search("PATH"))) == 0)
	{
		if (test_execpath(*(c->av.cav + c->var_)))
			return (1);
		c->cmd.c = (*c->av.cav + c->var_);
	}
	else
	{
		c->cmd = *z;
	}
	return (0);
}


int		main(int ac, char **av, char **envp)
{
	char	*s;
	t_lst	*t;
	t_command	*c;

	s = ft_readline("", 0);
	init_tokenizer();
	t = tokenize(s);
	t = parse_token(t);
	c = t->data;
//	dprintf(1, "%s\n", c->av.lav->data);
	transmute_av(c);
	init_htvar(envp);
	chash_init();
/////
	set_execpath(c);
	dprintf(1, "%d %d %d\n", SIGSTOP, SIGTSTP, c->endsym);
	// cut the pack of command into jobs of ; / &
	/////
/*****process parsed bullshit in a fork or not******/
}
