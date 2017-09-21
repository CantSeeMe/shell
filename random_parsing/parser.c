/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jye <jye@student.42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2017/06/14 20:41:29 by jye               #+#    #+#             */
/*   Updated: 2017/09/21 20:21:57 by jye              ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <stdlib.h>
#include <fcntl.h>

#include "command.h"
#include "token.h"
#include "lst.h"

#include <string.h>
#include <stdio.h>

int		redir_token(t_token *to, t_lst *argv_token)
{
	return (to->sym >= greater ||
			(to->sym == number && to->symbreak && argv_token->next));
}

void	parse_redir(t_lst **argv_token, t_lst **redir)
{
	t_rd *rd;

	if ((rd = get_redirection(argv_token)) == 0)
	{
		dummy_redirection(argv_token);
		return ;
	}
	if (rd->o_flag == -1 && rd->type == RDF_IN)
		here_tag(rd);
	if (!*redir ||
		(rd->o_flag == -1 && rd->type == RDF_IN && rd->heretag == -2) ||
		append_lst__(*redir, rd))
	{
		free(rd->s);
		free(rd);
		return ;
	}
	*redir = (*redir)->next;
	if (rd->o_flag == -1 && rd->type == RDF_IN && rd->heretag == -1)
		return (1);
	return (0);
}

void	parse_argv(t_lst **av, t_lst **argv_token, char *s)
{
	if (!(push_lst__(av, s)))
		co->ac += 1;
	else
		free(s);
	pop_lst__(argv_token, free);
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
			r = parse_redir(&argv_token, &redir);
		else
			parse_argv(&co->av.lav, &argv_token, s);
	}
	if (co->redir)
		pop_lst__(&co->redir, NULL);
	if (r)
		while (argv_token)
			pop_lst__(&argv_token, free_token);
	return (r);
}
