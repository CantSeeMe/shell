/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   base.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jye <jye@student.42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2017/08/16 04:00:33 by jye               #+#    #+#             */
/*   Updated: 2017/08/21 20:50:25 by root             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "hashlib.h"
#include "ft_readline.h"
#include "token.h"
#include "parser.h"
#include "lst.h"
#include "htcmd.h"
#include "htvar.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

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

int		main(int ac, char **av, char **envp)
{
	char	*s;
	t_lst	*t;
	t_command	*c;
	t_ccsh	*z;

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
	z = chash_lookup(*c->av.cav, vhash_search("PATH"));
	if (z == 0)
	{
		if (strchr(*c->av.cav, '/'))
		{
			if (!access(*c->av.cav, X_OK))
			{
				c->cmd.c = *c->av.cav;
			}
		}
	}
	else
	{
		c->cmd = *z;
	}
	dprintf(1, "%s\n", c->cmd.c);
/*****process parsed bullshit in a fork or not******/
	
	return (0);
}
