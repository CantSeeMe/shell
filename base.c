/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   base.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jye <jye@student.42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2017/08/16 04:00:33 by jye               #+#    #+#             */
/*   Updated: 2017/09/19 00:52:09 by root             ###   ########.fr       */
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

int		main(int ac, char **av, char **envp)
{
	char	*s;
	char	*e;
	t_lst	*t;

	init_htvar(envp);
	chash_init();
	init_tokenizer();
	while (1)
	{
		s = ft_readline("minishell> ", strlen("minishell> "));
		if (s == NULL || s == (char *)-1)
			exit(127);
		e = transmute_exp_spec(s);
		t = tokenize(e);
		if (e != 0)
			free(e);
		t = parse_token(t);
		while (t)
		{
			t_job	*job;
			job = job_create(&t);
			job_exec(job);
		}
		exit(1);
	}
}
