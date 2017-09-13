/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jye <jye@student.42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2017/06/14 20:41:29 by jye               #+#    #+#             */
/*   Updated: 2017/09/13 14:53:17 by root             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <stdlib.h>
#include <fcntl.h>

#include "hashlib.h"
#include "command.h"
#include "token.h"

//to be removed
#include <string.h>
#include <stdio.h>

#include "ft_readline.h"
#include "etc_parse.h"

int			syntax_check(t_lst *tokens)
{
	t_token	*to;
	t_lst	*last;
	char	*exp;
	char	*prompt;
	t_token	stack;

	if (tokens == (t_lst *)0)
		return (1);
	stack = (t_token){.s = (((t_token *)tokens->data)->s),
					  .sym = (((t_token *)tokens->data)->sym),
					  .symbreak = 0};
	while (42)
	{
		while (tokens)
		{
			to = (t_token *)tokens->data;
			if (to->sym <= number)
				stack = ((t_token){.s = 0, .sym = 0, .symbreak = 0});
			else
			{
				if (stack.sym >= number)
				{
					parse_error(stack.s, ERROR_EXPECTED);
					return (1);
				}
				stack = ((t_token){.s = to->s, .sym = to->sym, .symbreak = 0});
			}
			last = tokens;
			tokens = tokens->next;
		}
		if (stack.sym != ampersand && stack.sym != period && stack.sym != 0)
		{
			if ((stack.sym > period && stack.sym < greater))
			{
				prompt = ((char*[]){"cmdand> ", "cmdor> ", "pipe> "})[stack.sym - andsym];
				while ((exp = ft_readline(prompt, strlen(prompt))) &&
					   (exp != 0 && exp != (char *)-1))
				{
					exp = transmute_exp_spec(exp);
					if ((tokens = tokenize(exp)) == 0)
					{
						free(exp);
						exp = 0;
						continue ;
					}
					free(exp);
					break ;
				}
				if (exp == 0 || exp == (char *)-1)
				{
					if (exp == 0)
					{}// unexpected end of line
					return (1);
				}
				last->next = tokens;
				continue ;
			}
			else
			{
				//unexpected token newline
				return (1);
			}
		}
		return (0);
	}
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
	co->ac = 0;
	co->endsym = period;
	return (co);
}

t_command	*get_command(t_lst **tokens)
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

t_lst		*split_token(t_lst *tokens)
{
	t_lst		*clst;
	t_lst		*cp;
	t_command	*co;

	if ((clst = init_lst__(NULL)) == 0)
	{
		while (tokens)
			pop_lst__(&tokens, free_token);
		return (0);
	}
	cp = clst;
	while ((co = get_command(&tokens)))
	{
		if ((append_lst__(cp, co)))
		{
			while (co->av.lav)
				pop_lst__(&co->av.lav, free_token);
			free(co);
		}
		else
			cp = cp->next;
	}
	pop_lst__(&clst, 0);
	if (tokens)
	{
		while (tokens)
			pop_lst__(&tokens, free_token);
		if (clst && cp->data)
		{
			co = cp->data;
			co->endsym = period;
		}
	}
	return (clst);
}

int		redir_token(t_token *to, t_lst *argv_token)
{
	return (to->sym >= greater ||
			(to->sym == number && to->symbreak && argv_token->next));
}

int		parse_redir(t_lst **argv_token, t_lst **redir)
{
	t_rdtype *rd;

	if ((rd = get_redirection(argv_token)) == 0)
	{
		dummy_redirection(argv_token);
		return (0);
	}
	if (rd->fd_.o_flag == -1 && rd->type == RDF_STDIN)
		here_tag(rd);
	if (!*redir ||
		append_lst__(*redir, rd))
	{
		free(rd->fd_.s);
		free(rd);
		return (0);
	}
	*redir = (*redir)->next;
	if (rd->fd_.o_flag == -1 && rd->fd_.heretag == -1)
	{
		return (1);
	}
	return (0);
}

int		parse_command(t_command *co)
{
	t_lst		*redir;
	t_lst		*argv_token;
	t_token		*to;
	int			r;

	argv_token = co->av.lav;
	co->av.lav = 0;
	co->redir = init_lst__(NULL);
	redir = co->redir;
	r = 0;
	while (!r && argv_token)
	{
		to = (t_token *)argv_token->data;
		if (redir_token(to, argv_token))
		{
			r = parse_redir(&argv_token, &redir);
		}
		else
		{
			co->ac += !push_lst__(&co->av.lav, to->s);
			pop_lst__(&argv_token, free);
		}
	}
	if (co->redir)
		pop_lst__(&co->redir, NULL);
	if (r)
		while (argv_token)
			pop_lst__(&argv_token, free_token);
	return (r);
}

t_lst	*parse_token(t_lst *tokens)
{
	t_lst	*clst;
	t_lst	*h;

	if (syntax_check(tokens))
	{
		while (tokens)
		{
			free(((t_token *)tokens->data)->s);
			pop_lst__(&tokens, free);
		}
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
		clst = clst->next;
		while (clst)
		{
			t_command *c;

			c = clst->data;
			while (c->av.lav)
				pop_lst__(&c->av.lav, free_token);
			pop_lst__(&clst, free);
		}
		while (h)
		{
			t_command *c;

			c = h->data;
			while (c->av.lav)
				pop_lst__(&c->av.lav, free);
			while (c->redir)
			{
				free(((t_rdtype *)c->redir->data)->fd_.s);
				pop_lst__(&c->redir, free);
			}
			pop_lst__(&h, free);
		}
	}
	return (h);
}
