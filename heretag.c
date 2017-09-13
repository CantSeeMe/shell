/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   heretag.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: root <marvin@42.fr>                        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2017/09/13 12:36:28 by root              #+#    #+#             */
/*   Updated: 2017/09/13 14:25:08 by root             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <string.h>
#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <fcntl.h>
#include "command.h"
#include "ft_readline.h"

int		is_heretag(char *s, char *heretag)
{
	int		r;
	size_t	sl;

	r = 0;
	while (*s && *s == ' ')
		s++;
	sl = strlen(heretag);
	if (!strncmp(s, heretag, sl))
	{
		dprintf(1, "HELLO\n");
		s += sl;
		r = 1;
	}
	while (*s && *s == ' ')
		s++;
	if (*s != 0)
	{
		r = 0;
	}
	return (r);
}

void	here_tag(t_rdtype *rd)
{
	char	*s;
	int		fd[2];

	dprintf(1, "%s\n", rd->fd_.s);
	rd->fd_.heretag = -1 + pipe(fd);
	if (rd->fd_.heretag == -2)
		return ;
	while ((s = ft_readline("heretag> ", strlen("heretag> "))) &&
		   s != (char *)-1)
	{
		if (is_heretag(s, rd->fd_.s))
			break ;
		write(fd[1], s, strlen(s));
		write(fd[1], "\n", 1);
		free(s);
	}
	close(fd[1]);
	free(rd->fd_.s);
	rd->fd_.s = 0;
	if (s == (char *)-1)
	{
		close(fd[0]);
		return ;
	}
	free(s);
	rd->fd_.heretag = fd[0];
}
