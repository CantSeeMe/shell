/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   job5.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jye <jye@student.42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2017/09/21 20:52:46 by jye               #+#    #+#             */
/*   Updated: 2017/10/29 15:45:58 by jye              ###   ########.fr       */
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

int			job_openfd_(t_rd *rd)
{
	if (rd->type == RDF_OUT)
	{
		if ((job_outopen(rd)))
			return (1);
	}
	else if (rd->type == RDF_IN)
	{
		if ((job_inopen(rd)))
			return (1);
	}
	else if (rd->type >= RDF_RDOUT)
	{
		if ((job_rdiropen(rd)))
			return (1);
	}
	return (0);
}

t_lst		*job_openfd(t_lst *redir)
{
	t_rd	*rd;
	t_lst	*rest;

	rest = 0;
	while (redir)
	{
		rd = (t_rd *)redir->data;
		if (job_openfd_(rd))
			break ;
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
