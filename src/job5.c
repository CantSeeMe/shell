/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   job5.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jye <jye@student.42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2017/09/21 20:52:46 by jye               #+#    #+#             */
/*   Updated: 2017/10/08 21:35:15 by jye              ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "lst.h"
#include "job.h"
#include "command.h"
#include "libft.h"

#include <fcntl.h>
#include <unistd.h>
#include <stdlib.h>
#include <sys/stat.h>

static int	job_outopencheck(const char *s)
{
	struct stat	fstat;

	return (!stat(s, &fstat) && S_ISDIR(fstat.st_mode));
}

int			job_outopen(t_rd *rd)
{
	int		fd;

	if (ft_strcmp(rd->s, "-"))
	{
		if (job_outopencheck(rd->s) ||
			(fd = open(rd->s, rd->o_flag, 0644)) == -1)
		{
			rd->save = -1;
			return (1);
		}
	}
	else
		fd = -1;
	rd->save = dup(rd->fd);
	if (fd != -1)
	{
		dup2(fd, rd->fd);
		close(fd);
	}
	else
		close(rd->fd);
	return (0);
}

static int	job_inopencheck(const char *s)
{
	struct stat	fstat;

	return (stat(s, &fstat) || S_ISDIR(fstat.st_mode));
}

int			job_inopen(t_rd *rd)
{
	int			fd;

	if (rd->s == 0 && rd->o_flag == -1)
	{
		if (rd->heretag < 1)
			return (1);
		rd->save = dup(rd->fd);
		dup2(rd->heretag, rd->fd);
		close(rd->heretag);
		rd->heretag = -1;
	}
	else
	{
		if (job_inopencheck(rd->s) || (fd = open(rd->s, rd->o_flag)) == -1)
		{
			rd->s = 0;
			return (1);
		}
		rd->save = dup(rd->fd);
		dup2(fd, rd->fd);
		close(fd);
	}
	return (0);
}

static void	job_rdiropen(t_rd *rd)
{
	int	fd;

	fd = ft_atoi(rd->s);
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
		if (rd->type == RDF_OUT)
		{
			if ((job_outopen(rd)))
				break ;
		}
		else if (rd->type == RDF_IN)
		{
			if ((job_inopen(rd)))
				break ;
		}
		else if (rd->type == RDF_RDIR)
			job_rdiropen(rd);
		pop_lst__(&redir, 0);
		push_lst__(&rest, rd);
	}
	if (redir)
	{
		job_restorefd(redir);
		job_restorefd(rest);
		rest = (t_lst *)(-1);
	}
	return (rest);
}

void		job_restorefd(t_lst *rest)
{
	t_rd	*rd;

	while (rest)
	{
		rd = (t_rd *)rest->data;
		free(rd->s);
		if (rd->save > 0)
		{
			dup2(rd->save, rd->fd);
			close(rd->save);
		}
		pop_lst__(&rest, free);
	}
}
