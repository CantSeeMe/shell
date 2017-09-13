/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   base.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jye <jye@student.42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2017/08/16 04:00:33 by jye               #+#    #+#             */
/*   Updated: 2017/09/13 14:19:19 by root             ###   ########.fr       */
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
#include "etc_parse.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <signal.h>
#include <unistd.h>


int		transmute_av(t_command *c)
{
	char	**av;
	char	*dollar;
	t_lst	*z;
	int		i;

	if ((av = malloc(sizeof(*av) * (c->ac + 1))) == 0)
		return (1);
	z = c->av.lav;
	i = c->ac;
	av[i--] = 0;
	while (z)
	{
		dollar = transmute_dollar((char *)z->data);
		av[i--] = dollar ? dollar : z->data;
		pop_lst__(&z, dollar ? free : 0);
	}
	c->av.cav = av;
	return (0);
}

/******************************/
/******************************/
/******************************/

#define WORD_UCHAR	"ABCDEFGHIJKLMNOPQRSTUVWXYZ"
#define WORD_LCHAR	"abcdefghijklmnopqrstuvwxyz"
#define WORD_SCHAR	"._-"
#define WORD_NCHAR	"1234567890"

#define WORD_ANCHAR	WORD_UCHAR WORD_LCHAR WORD_NCHAR


int		check_varname(char *s)
{
	while (*s != '=')
	{
		if (!strchr(WORD_ANCHAR, *s))
			return (0);
		s++;
	}
	return (1);
}

int		set_execpath(t_command *c)
{
	t_ccsh		*z;
	t_var		*v;

	if (strchr(*c->av.cav, '=') && check_varname(*c->av.cav))
	{
		if ((v = init_var(*c->av.cav, HTVAR_VAR_GLOBAL)))
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
	vhash_set_underscore(HTVAR_SET_PATH, c);
	while (envp)
	{
		v = envp->data;
		c->envp[i--] = defrag_var(v->key, v->value);
		envp = envp->next;
	}
	return (0);
}

int		main(int ac, char **av, char **envp)
{
	char	*s;
	char	*e;
	t_lst	*t;
	t_command	*c;

	init_htvar(envp);
	chash_init();
	init_tokenizer();
	while (1)
	{
		s = ft_readline("minishell> ", strlen("minishell> "));
		if (s == NULL || s == (char *)-1)
			exit(127);
//		dprintf(1, "%p\n", s);
		e = transmute_exp_spec(s);
		/* if (e) */
		/* { */
		/* 	size_t lul; */
		/* 	lul = strlen(e); */
		/* 	for (int i = 0; i < lul; i++) */
		/* 		dprintf(1, "%hhd ", e[i]); */
		/* 	dprintf(1, "\n"); */
		/* } */
		t = tokenize(e);
		if (e != 0)
			free(e);
		t = parse_token(t);
		t_lst	*cp = t;
		while (cp)
		{
			c = cp->data;
			transmute_av(c);
			set_execpath(c);
			dprintf(1, "%s\n", *c->av.cav);
			set_envp(c);
			cp = cp->next;
		}
		while (t)
		{
			t_job	*job;
			job = job_create(&t);
			job_exec(job);
			
		}
		exit(1);
	}
	/////
/*****process parsed bullshit in a fork or not******/
}
