/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   heretag.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: root <marvin@42.fr>                        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2017/09/13 12:36:28 by root              #+#    #+#             */
/*   Updated: 2017/11/15 03:27:54 by jye              ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "command.h"
#include "ft_readline.h"
#include "libft.h"
#include "ft_printf.h"

#include <unistd.h>
#include <stdlib.h>
#include <fcntl.h>

int		is_heretag(char *s, char *heretag)
{
	size_t	sl;
	int		i;

	while (*s && *s == ' ')
		s++;
	sl = ft_strlen(heretag);
	if ((i = !ft_strncmp(s, heretag, sl)))
		s += sl;
	while (*s && *s == ' ')
		s++;
	return (i && *s == 0);
}

void	here_tag(t_rd *rd)
{
	char	*s;
	int		fd[2];

	rd->heretag = -1 + pipe(fd);
	if (rd->heretag == -2)
		return ;
	while ((s = ft_readline("heretag> ", 9)) &&
			s != (char *)-1)
	{
		if (is_heretag(s, rd->s))
		{
			break ;
		}
		write(fd[1], s, ft_strlen(s));
		write(fd[1], "\n", 1);
		free(s);
	}
	close(fd[1]);
	free(rd->s);
	rd->s = 0;
	if (s == (char *)-1)
		close(fd[0]);
	else
		free(s);
	rd->heretag = (s == (char *)-1) ? -1 : fd[0];
}
