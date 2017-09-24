/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser2.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jye <jye@student.42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2017/09/21 19:50:50 by jye               #+#    #+#             */
/*   Updated: 2017/09/24 02:41:21 by jye              ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "command.h"
#include "lst.h"
#include "token.h"

#include <stdlib.h>

static t_command	*init_command(void)
{
	t_lst		*l;
	t_command	*co;

	if ((l = init_lst__(NULL)) == 0)
		return ((t_command *)0);
	if ((co = malloc(sizeof(*co))) == 0)
	{
		free(l);
		return ((t_command *)0);
	}
	co->av.lav = l;
	co->ac = 0;
	co->endsym = period;
	return (co);
}

static t_command	*get_command(t_lst **tokens)
{
	t_lst		*av;
	t_command	*co;
	t_token		*to;

	if (*tokens == 0 || (co = init_command()) == 0)
		return ((t_command *)0);
	av = co->av.lav;
	while (*tokens)
	{
		to = (t_token *)(*tokens)->data;
		if (to->sym > number && to->sym < greater)
		{
			co->endsym = to->sym;
			pop_lst__(tokens, free_token);
			break ;
		}
		pop_lst__(tokens, 0);
		if ((append_lst__(av, to)))
			free_token(to);
		else
			av = av->next;
	}
	pop_lst__(&co->av.lav, 0);
	return (co);
}

static void			free_splited_token(t_command *c)
{
	while (c->av.lav)
		pop_lst__(&c->av.lav, free_token);
	free(c);
}

static void			free_aborted_token(t_lst **tokens, t_lst *clst, t_lst *cp)
{
	t_command	*co;

	while (*tokens)
		pop_lst__(tokens, free_token);
	if (clst && cp->data)
	{
		co = cp->data;
		co->endsym = period;
	}
}

t_lst				*split_token(t_lst *tokens)
{
	t_lst		*clst;
	t_lst		*cp;
	t_command	*co;

	if ((clst = init_lst__(0)) == 0)
	{
		while (tokens)
			pop_lst__(&tokens, free_token);
		return (0);
	}
	cp = clst;
	while ((co = get_command(&tokens)))
	{
		if ((append_lst__(cp, co)))
			free_splited_token(co);
		else
			cp = cp->next;
	}
	pop_lst__(&clst, 0);
	if (tokens)
		free_aborted_token(&tokens, clst, cp);
	return (clst);
}
