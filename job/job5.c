/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   job5.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jye <jye@student.42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2017/09/21 20:52:46 by jye               #+#    #+#             */
/*   Updated: 2017/09/21 21:01:31 by jye              ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "lst.h"
#include "command.h"

#include <fcntl.h>
#include <unistd.h>

static void	job_outopen(t_rd *rd)
{
	int		fd;

	if ((fd = open(rd->s, rd->o_flag, 0644)) == -1)
	{
		free(rd->s);
		rd->save = -1;
		return ;
	}
	free(rd->s);
	rd->save = dup(rd->fd);
	dup2(fd, rd->fd);
	close(fd);
}

static void	job_inopen(t_rd *rd)
{
	int			fd;

	if (rd->s == 0 && rd->o_flag == -1)
	{
		if (rd->heretag == -2)
			return ;
		rd->save = dup(rd->fd);
		dup2(rd->heretag, rd->fd);
		close(rd->heretag);
		rd->heretag = -1;
	}
	else
	{
		if ((fd = open(rd->s, rd->o_flag)) == -1)
		{
			free(rd->s);
			return ;
		}
		free(rd->s);
		rd->save = dup(rd->fd);
		dup2(fd, rd->fd);
		close(fd);
	}
}

static void	job_rdiropen(t_rd *rd)
{
	int	fd;

	fd = rd->s ? atoi(rd->s) : -1;
	free(rd->s);
	if (fd == -1)
		return ;
	rd->save = dup(rd->fd);
	dup2(fd, rd->fd);
}

t_lst		*job_openfd(t_lst *redir)
{
	t_rd	*rd;
	t_lst	*rest;

	rest = 0;
	while (redir)
	{
		rd = (t_rd *)redir->data;
		pop_lst__(&redir, 0);
		push_lst__(&rest, rd);
		if (rd->type == RDF_OUT)
		{
			job_outopen(rd);
		}
		else if (rd->type == RDF_IN)
		{
			job_inopen(rd);
		}
		else if (rd->type == RDF_RDIR)
		{
			job_rdiropen(rd);
		}
	}
	return (rest);
}

void		job_restorefd(t_lst *rest)
{
	t_rd	*rd;

	while (rest)
	{
		rd = (t_rd *)rest->data;
		dup2(rd->save, rd->fd);
		close(rd->save);
		pop_lst__(&rest, free);
	}
}
