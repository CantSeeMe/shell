/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser3.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jye <jye@student.42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2017/09/21 20:01:50 by jye               #+#    #+#             */
/*   Updated: 2017/09/21 20:30:50 by jye              ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "command.h"
#include "lst.h"
#include <stdlib.h>

static void	abort_non_parsed(t_lst *clst)
{
	t_command *c;

	while (clst)
	{
		c = (t_command *)clst->data;
		while (c->av.lav)
			pop_lst__(&c->av.lav, free_token);
		pop_lst__(&clst, free);
	}
}

static void	abort_parsed(t_lst *h)
{
	while (h)
	{
		c = h->data;
		while (c->av.lav)
			pop_lst__(&c->av.lav, free);
		while (c->redir)
		{
			free(((t_rd *)c->redir->data)->s);
			pop_lst__(&c->redir, free);
		}
		pop_lst__(&h, free);
	}
}

static void	abort_all(t_lst *tokens)
{
	while (tokens)
	{
		free(((t_token *)tokens->data)->s);
		pop_lst__(&tokens, free);
	}
}

t_lst		*parse_token(t_lst *tokens)
{
	t_lst	*clst;
	t_lst	*h;

	if (syntax_check(tokens))
	{
		abort_all(tokens);
		return (0);
	}
	clst = split_token(tokens);
	h = clst;
	while (clst)
	{
		if (parse_command((t_command *)clst->data))
			break ;
		clst = clst->next;
	}
	if (clst)
	{
		abort_non_parsed(clst->next);
		abort_parsed(h);
		return ((t_lst *)0);
	}
	return (h);
}
