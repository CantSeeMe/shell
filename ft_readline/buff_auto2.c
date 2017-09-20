/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   buff_auto2.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jye <jye@student.42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2017/09/20 18:42:19 by jye               #+#    #+#             */
/*   Updated: 2017/09/20 19:02:07 by jye              ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "buff_auto.h"
#include <stdio.h>
#include <signal.h>
#include <sys/stat.h>

static int	buff_confirm_show(size_t nb_file)
{
	char	s;

	dprintf(2, "\n\rDisplay %lu possibilities ? (y or n)", nb_file);
	while (42)
	{
		s = 0;
		read(0, &s, 1);
		if (s == 'n')
		{
			dprintf(2, "\n\r");
			write(2, g_prompt, strlen(g_prompt));
			buff_refresh(0, g_buffer.s, g_buffer.len);
			shift_cursor(g_buffer.len, g_buffer.cu);
			return (1);
		}
		else if (s == 'y')
			return (0);
		else if (s == 0x3)
		{
			reset_readline();
			return (1);
		}
	}
}

static int	buff_confirm_show_more(void)
{
	char	s;

	s = 0;
	dprintf(2, "--MORE--");
	read(0, &s, 1);
	if (s == 'q')
		return (1);
	else if (s == 0x3)
	{
		reset_readline();
		return (1);
	}
	dprintf(2, "\e[2K\r");
	return (0);
}

static void	buff_show_alike_(t_cdir *c, int skip_)
{
	int		row;
	int		i;
	int		z;

	row = g_winsize.row;
	i = 0;
	z = 0;
	while (z < (int)c->nb_file)
	{
		while (i < (int)c->nb_file)
		{
			dprintf(2, "%*s", -((int)c->maxlen), c->file[i]);
			i += skip_;
			z++;
		}
		i = (i % skip_) + 1;
		dprintf(2, "\n\r");
		signal(SIGWINCH, reload_line);
		if (row < 0 && buff_confirm_show_more())
			return ;
	}
}

void		buff_show_alike(t_cdir *c)
{
	int		z;
	int		skip_;

	if (c->nb_file <= 1)
		return ;
	if (c->nb_file >= 100 && buff_confirm_show(c->nb_file))
		return ;
	c->maxlen += MIN_COLUMN_WIDTH;
	skip_ = g_winsize.col / c->maxlen;
	skip_ = (c->nb_file / skip_) + 1;
	dprintf(2, "\n\r");
	buff_show_alike_(c, z, skip_);
	if (g_buffer.msize == 0)
		return ;
	write(2, g_prompt, strlen(g_prompt));
	buff_refresh(0, g_buffer.s, g_buffer.len);
	shift_cursor(g_buffer.len, g_buffer.cu);
}
