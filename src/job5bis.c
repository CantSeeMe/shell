/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   job5bis.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jye <jye@student.42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2017/10/16 03:42:08 by jye               #+#    #+#             */
/*   Updated: 2017/10/21 21:39:17 by jye              ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "lst.h"
#include "job.h"
#include "command.h"
#include "libft.h"
#include "ft_printf.h"

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

	if (job_outopencheck(rd->s) ||
		(fd = open(rd->s, rd->o_flag, 0644)) == -1)
	{
		ft_dprintf(2, "21sh: oops, can't run, can't do shit with %s\n",
				   rd->s);
		return (1);
	}
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
	return (access(s, R_OK));
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
			ft_dprintf(2, "21sh: oops, can't run, can't do shit with %s\n",
				rd->s);
			return (1);
		}
		rd->save = dup(rd->fd);
		dup2(fd, rd->fd);
		close(fd);
	}
	return (0);
}

int			job_rdiropen(t_rd *rd)
{
	int	fd;

	if (!ft_strcmp(rd->s, "-"))
		rd->o_flag = -2;
	if (rd->o_flag == 0)
	{
		ft_dprintf(2, "21sh: oops, can't run, can't do shit with %s\n", rd->s);
		return (1);
	}
	rd->save = dup(rd->fd);
	if (rd->o_flag == -2)
	{
		close(rd->fd);
	}
	else
	{
		fd = ft_atoi(rd->s);
		dup2(fd, rd->fd);
	}
	return (0);
}
