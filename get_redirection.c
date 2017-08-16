/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   get_redirection.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jye <jye@student.42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2017/06/28 19:21:56 by jye               #+#    #+#             */
/*   Updated: 2017/08/16 06:04:35 by jye              ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "parser.h"
#include "token.h"
#include <fcntl.h>
//
#include <stdlib.h>

static void	fill_rdtype(t_rdtype *rd, t_token *to)
{
	if (to->sym <= ggreater)
	{
		rd->fd_.o_flag = ((to->sym == greater) ? O_TRUNC : O_APPEND)
			| (O_WRONLY | O_CREAT);
		rd->rdtype = RDF_STDOUT;
	}
	else
	{
		rd->fd_.o_flag = ((to->sym == lower) ? O_RDONLY : -1);
		rd->rdtype = RDF_STDIN + (to->sym == greater_amp);
	}
}

static int	fill_fddata(t_rdtype *rd, t_token *to, int fd)
{
	if (to->sym != number && to->sym != word)
		return (1);
	rd->fd_.s = to->s;
	to->s = 0;
	if (fd == -1)
		rd->fd_.fd = (rd->rdtype == RDF_STDOUT);
	else
		rd->fd_.fd = fd;
	return (0);
}

t_rdtype	*get_redirection(t_lst **token)
{
	t_rdtype	*rd;
	t_token		*to;
	int			fd;

	if ((rd = malloc(sizeof(*rd))) == 0)
		return ((t_rdtype *)0);
	to = (t_token *)(*token)->data;
	if (to->sym == number)
	{
		fd = atoi(to->s);
		free(to->s);
		pop_lst__(token, free);
		to = (t_token *)(*token)->data;
	}
	else
		fd = -1;
	fill_rdtype(rd, to);
	free(to->s);
	pop_lst__(token, free);
	to = (t_token *)(*token)->data;
	fill_fddata(rd, to, fd);
	pop_lst__(token, free);
	return (rd);
}
