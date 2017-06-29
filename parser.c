/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jye <jye@student.42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2017/06/14 20:41:29 by jye               #+#    #+#             */
/*   Updated: 2017/06/29 17:22:49 by jye              ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <stdlib.h>
#include <fcntl.h>

#include "hashlib.h"
#include "parser.h"
#include "token.h"

//to be removed
#include <string.h>
#include <stdio.h>

int			syntax_check(t_lst *tokens)
{
	t_token	*stack;
	t_token	*to;

	while (tokens)
	{
		to = (t_token *)tokens->data;
		if (to->sym <= number)
			stack = (t_token *)0;
		else
		{
			if (stack)
			{
				parse_error(stack->s, ERROR_EXPECTED);
				return (1);
			}
			stack = to;
		}
		tokens = tokens->next;
	}
	if (stack && stack->sym != ampersand && stack->sym != period)
	{
		parse_error(stack->s, ERROR_EXPECTED);
		return (1);
	}
	return (0);
}

t_command	*init_command(void)
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
	return (co);
}

t_command	*get_command(t_lst **tokens)
{
	t_lst		*av;
	t_command	*co;
	t_token		*to;

	if (*tokens == 0)
		return ((t_command *)0);
	if ((co = init_command()) == 0)
		return ((t_command *)-1);
	av = co->av.lav;
	while (*tokens)
	{
		to = (t_token *)(*tokens)->data;
		if (to->sym > number && to->sym < greater)
		{
			co->endsym = to->sym;
			free(to->s);
			pop_lst__(tokens, free);
			break ;
		}
		pop_lst__(tokens, 0);
		append_lst__(av, to);
		av = av->next;
	}
	pop_lst__(&co->av.lav, 0);
	return (co);
}

t_lst		*split_token(t_lst *tokens)
{
	t_lst		*clst;
	t_lst		*cp;
	t_command	*co;

	clst = init_lst__(NULL);
	cp = clst;
	while ((co = get_command(&tokens)))
	{
		if (co == (t_command *)-1)
			break ;
		if ((append_lst__(cp, co)))
			break ;
		cp = cp->next;
	}
	pop_lst__(&clst, 0);
	if (tokens)
	{
		return (0);
	}
	return (clst);
}

void	parse_command(t_command *co)
{
	t_lst		*argv;
	t_lst		*argv_token;
	t_token		*to;
	t_rdtype	*rd;

	argv = 0;
	argv_token = co->av.lav;
	while (argv_token)
	{
		to = (t_token *)argv_token->data;
		if (to->sym >= greater ||
			(to->sym == number && to->symbreak && argv_token->next))
		{
			if ((rd = get_redirection(&argv_token)) == 0)
				continue ;
			push_lst__(&co->redir, rd);
		}
		else
		{
			if (!push_lst__(&argv, to->s))
				co->ac++;
			pop_lst__(&argv_token, free);
		}
	}
	co->av.lav = argv;
}

void	parse_token(t_lst *clst)
{
	while (clst)
	{
		parse_command((t_command *)clst->data);
		clst = clst->next;
	}
}

int		main(int ac, char **av)
{
	t_lst	*t;

	init_tokenizer();
	t = tokenize(av[1]);
	if (syntax_check(t))
		return (1);
	t = split_token(t);
	parse_token(t);
	exit(0);
}
