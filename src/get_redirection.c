/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   get_redirection.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jye <jye@student.42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2017/06/28 19:21:56 by jye               #+#    #+#             */
/*   Updated: 2017/10/18 02:22:55 by root             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "command.h"
#include "token.h"
#include "libft.h"

#include <fcntl.h>
#include <stdlib.h>

static void	fill_rdtype(t_rd *rd, t_token *to)
{
	if (to->sym <= ggreater)
	{
		rd->o_flag = ((to->sym == greater) ? O_TRUNC : O_APPEND)
			| (O_WRONLY | O_CREAT);
		rd->type = RDF_OUT;
	}
	else
	{
		rd->o_flag = ((to->sym == lower) ? O_RDONLY : -1);
		rd->type = RDF_IN + (to->sym >= lower_amp);
	}
}

static void	fill_fddata(t_rd *rd, t_token *to, int fd)
{
	rd->save = -1;
	rd->heretag = -2;
	rd->s = to->s;
	if (rd->type == RDF_RDIR && to->sym == word)
		rd->o_flag = 0;
	to->s = 0;
	if (fd == -1)
		rd->fd = (rd->type == RDF_OUT);
	else
		rd->fd = fd;
}

t_rd		*get_redirection(t_lst **token)
{
	t_rd	*rd;
	t_token	*to;
	int		fd;

	if ((rd = malloc(sizeof(*rd))) == 0)
		return ((t_rd *)0);
	ft_memset(rd, 0, sizeof(*rd));
	to = (t_token *)(*token)->data;
	if (to->sym == number)
	{
		fd = ft_atoi(to->s);
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

void		dummy_redirection(t_lst **token)
{
	t_token		*to;

	to = (t_token *)(*token)->data;
	if (to->sym == number)
	{
		free(to->s);
		pop_lst__(token, free);
		to = (t_token *)(*token)->data;
	}
	free(to->s);
	pop_lst__(token, free);
	to = (t_token *)(*token)->data;
	free(to->s);
	pop_lst__(token, free);
}
