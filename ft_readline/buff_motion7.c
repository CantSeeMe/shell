/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   buff_motion7.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jye <jye@student.42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2017/09/20 19:09:44 by jye               #+#    #+#             */
/*   Updated: 2017/09/20 19:18:00 by jye              ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft_readline.h"
#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <curses.h>
#include <term.h>
#include <dirent.h>

void	buff_record(int start, ssize_t bufsize, int action)
{
	t_record	*r;

	if (!bufsize)
		return ;
	if ((r = malloc(sizeof(*r))) == 0)
		return ;
	r->start = start;
	if ((r->bufsize = bufsize) > 0)
	{
		if ((r->buf = malloc(r->bufsize + 1)) == 0)
		{
			free(r);
			return ;
		}
		memcpy(r->buf, g_buffer.s + start, r->bufsize);
		r->buf[r->bufsize] = 0;
	}
	else
		r->buf = 0;
	r->action = action;
	if (push_lst__(&g_record, r))
	{
		free(r->buf);
		free(r);
	}
}

void	buff_revert_write(t_record *r)
{
	shift_cursor(g_buffer.cu, r->start);
	memmove(g_buffer.s + r->start + r->bufsize,
			g_buffer.s + r->start,
			g_buffer.len - r->start);
	memcpy(g_buffer.s + r->start,
			r->buf,
			r->bufsize);
	g_buffer.len += r->bufsize;
	buff_refresh(r->start,
					g_buffer.s + r->start,
					g_buffer.len - r->start);
	g_buffer.cu = r->action & RL_CURSOR_MASK ?
		r->start : r->start + r->bufsize;
	shift_cursor(g_buffer.len, g_buffer.cu);
}

void	buff_revert_delete(t_record *r)
{
	memmove(g_buffer.s + r->start + r->bufsize,
			g_buffer.s + r->start,
			g_buffer.len - r->start);
	memset(g_buffer.s + g_buffer.len + r->bufsize, ' ',
			-(r->bufsize));
	shift_cursor(g_buffer.cu, r->start + r->bufsize);
	g_buffer.cu = r->start + r->bufsize;
	buff_refresh(g_buffer.cu,
					g_buffer.s + g_buffer.cu,
					g_buffer.len - g_buffer.cu);
	shift_cursor(g_buffer.len, g_buffer.cu);
	g_buffer.len += r->bufsize;
}

void	buff_revert_overwrite(t_record *r)
{
	memcpy(g_buffer.s + r->start, r->buf, r->bufsize);
	shift_cursor(g_buffer.cu, r->start);
	buff_refresh(r->start, g_buffer.s + r->start, r->bufsize);
	shift_cursor(r->start + r->bufsize, r->start);
	g_buffer.cu = r->start;
}

void	buff_revert(void)
{
	static void (*act[3])(t_record *) = {
		buff_revert_write,
		buff_revert_delete,
		buff_revert_overwrite
	};
	t_record	*r;

	if (!g_record)
		return ;
	r = (t_record *)g_record->data;
	act[r->action & 0xff](r);
	free(r->buf);
	pop_lst__(&g_record, free);
}
