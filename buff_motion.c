/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   buff_motion.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: root <marvin@42.fr>                        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2017/07/17 10:51:14 by root              #+#    #+#             */
/*   Updated: 2017/08/15 21:01:44 by jye              ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#define WORD_UCHAR	"ABCDEFGHIJKLMNOPQRSTUVWXYZ"
#define WORD_LCHAR	"abcdefghijklmnopqrstuvwxyz"
#define WORD_NCHAR	"1234567890"

#define WORD_ANCHAR	WORD_UCHAR WORD_LCHAR WORD_NCHAR

#include "prompt.h"
#include <string.h>
#include <stdio.h>
#include <stdlib.h>

void	buff_head(void)
{
	shift_cursor(g_cubuf, 0);
	g_cubuf = 0;
}

void	buff_end(void)
{
	shift_cursor(g_cubuf, g_buffer.len);
	g_cubuf = g_buffer.len;
}

void	buff_prev(void)
{
	shift_cursor(g_cubuf, g_cubuf - (g_cubuf > 0));
	g_cubuf -= g_cubuf > 0;
}

void	buff_next(void)
{
	shift_cursor(g_cubuf, g_cubuf + (g_cubuf < g_buffer.len));
	g_cubuf += g_cubuf < g_buffer.len;
}

void	buff_next_word(void)
{
	char	*s;
	size_t	i;

	s = g_buffer.s;
	s[g_buffer.len] = 0;
	i = g_cubuf;
	while (!strchr(WORD_ANCHAR, i[s]))
	{
		if (i == g_buffer.len)
			break ;
		i++;
	}
	while (strchr(WORD_ANCHAR, i[s]))
	{
		if (i == g_buffer.len)
			break ;
		i++;
	}
	shift_cursor(g_cubuf, i);
	g_cubuf = i;
}

void	buff_prev_word(void)
{
	char	*s;
	int		i;

	s = g_buffer.s;
	if ((i = g_cubuf - (g_cubuf > 0)) == -1)
		return ;
	while (!strchr(WORD_ANCHAR, i[s]))
	{
		if (i <= 0)
			break ;
		i--;
	}
	while (strchr(WORD_ANCHAR, i[s]))
	{
		if (i <= 0)
			break ;
		i--;
	}
	shift_cursor(g_cubuf, i + (i > 0));
	g_cubuf = i + (i > 0);
}

void	buff_del_prev(void)
{
	if (g_cubuf == 0)
		return ;
	g_cubuf -= 1;
	buff_record(g_cubuf, 1, RL_ACTION_WRITE);
	memcpy(g_buffer.s + g_cubuf,
		   g_buffer.s + g_cubuf + 1,
		   g_buffer.len - g_cubuf);
	shift_cursor(g_cubuf + 1, g_cubuf);
	g_buffer.len -= 1;
	g_buffer.s[g_buffer.len] = ' ';
	buff_refresh(g_cubuf,
				 g_buffer.s + g_cubuf,
				 g_buffer.len - g_cubuf + 1);
	shift_cursor(g_buffer.len + 1, g_cubuf);
}

void	buff_del_next(void)
{
	if (g_cubuf == g_buffer.len)
		return ;
	buff_record(g_cubuf, 1, RL_ACTION_WRITE | RL_CURSOR_KEEP);
	memcpy(g_buffer.s + g_cubuf,
		   g_buffer.s + g_cubuf + 1,
		   g_buffer.len - g_cubuf);
	g_buffer.len -= 1;
	g_buffer.s[g_buffer.len] = ' ';
	buff_refresh(g_cubuf,
				 g_buffer.s + g_cubuf,
				 g_buffer.len - g_cubuf + 1);
	shift_cursor(g_buffer.len + 1, g_cubuf);
}

void	buff_del_word(void)
{
	int		cubuf;
	int		diff;

	cubuf = g_cubuf;
	buff_prev_word();
	diff = cubuf - g_cubuf;
	buff_record(g_cubuf, diff, RL_ACTION_WRITE);
	g_buffer.len -= diff;
	memcpy(g_buffer.s + g_cubuf,
		   g_buffer.s + cubuf,
		   g_buffer.len - g_cubuf);
	memset(g_buffer.s + g_buffer.len, ' ', diff);
	buff_refresh(g_cubuf, g_buffer.s + g_cubuf, g_buffer.len - g_cubuf + diff);
	shift_cursor(g_buffer.len + diff, g_cubuf);
}

void	buff_lowtoup(void)
{
	char	*s;
	int		i;
	int		diff;

	s = g_buffer.s + g_cubuf;
	g_buffer.s[g_buffer.len] = 0;
	diff = 0;
	while (!strchr(WORD_ANCHAR, s[diff]))
		diff++;
	i = diff;
	while (s[diff] && strchr(WORD_ANCHAR, s[diff]))
		diff++;
	buff_record(g_cubuf, diff,
				RL_ACTION_OVERWRITE | RL_CURSOR_KEEP);
	while (i < diff)
	{
		if (s[i] >= 0x60 && s[i] <= 0x7a)
			s[i] -= 0x20;
		++i;
	}
	buff_refresh(g_cubuf, g_buffer.s + g_cubuf, diff);
	g_cubuf += diff;
}
	
void	buff_uptolow(void)
{
	char	*s;
	int		diff;
	int		i;

	s = g_buffer.s + g_cubuf;
	g_buffer.s[g_buffer.len] = 0;
	diff = 0;
	while (!strchr(WORD_ANCHAR, s[diff]))
		diff++;
	i = diff;
	while (s[diff] && strchr(WORD_ANCHAR, s[diff]))
		diff++;
	buff_record(g_cubuf, diff,
				RL_ACTION_OVERWRITE | RL_CURSOR_KEEP);
	while (i < diff)
	{
		if (s[i] >= 0x40 && s[i] <= 0x5a)
			s[i] += 0x20;
		i++;
	}
	buff_refresh(g_cubuf, g_buffer.s + g_cubuf, diff);
	g_cubuf += diff;
}

void	buff_capitalize(void)
{
	char	*s;
	int		diff;
	int		i;

	g_buffer.s[g_buffer.len] = 0;
	s = g_buffer.s + g_cubuf;
	diff = 0;
	while (!strchr(WORD_ANCHAR, s[diff]))
		diff++;
	i = diff;
	while (s[diff] && strchr(WORD_ANCHAR, s[diff]))
		diff++;
	buff_record(g_cubuf, diff,
				RL_ACTION_OVERWRITE | RL_CURSOR_KEEP);
	if (s[i] >= 0x60 && s[i] <= 0x7a)
		s[i] -= 0x20;
	i += 1;
	while (i < diff)
	{
		if (s[i] >= 0x40 && s[i] <= 0x5a)
			s[i] += 0x20;
		++i;
	}
	buff_refresh(g_cubuf, g_buffer.s + g_cubuf, diff);
	g_cubuf += diff;
}

/*********************/

int		buff_malloc(size_t s)
{
	if ((g_buffer.s = malloc(s)) == 0)
		return (1);
	g_buffer.msize = s;
	return (0);
}

int		buff_realloc(size_t s)
{
	char	*t;

	t = g_buffer.s;
	if (buff_malloc(s))
	{
		g_buffer.s = t;
		return (1);
	}
	memcpy(g_buffer.s, t, g_buffer.len);
	free(t);
	return (0);
}

void	buff_refresh(int cursor, char *s, ssize_t slen)
{
	static char	q[] = {0, ' ', '\r'};
	int			col;

	col = (cursor + g_psize) % g_winsize.col;
	while (slen--)
	{
		q[0] = *s++;
		++col;
		if (col >= g_winsize.col)
		{
			write(STDERR_FILENO, q, 3);
			col = 0;
		}
		else
		{
			write(STDERR_FILENO, q, 1);
		}
	}
}

void	buff_newline(void)
{
	int	diff;

	diff = ((g_psize + g_buffer.len) / g_winsize.col) + 1;
	diff -= (g_psize + g_cubuf) / g_winsize.col;
	while (diff--)
		write(STDERR_FILENO, "\n", 1);
	write(STDERR_FILENO, "\r", 1);
}

void	buff_delete(void)
{
	if (g_buffer.len == 0)
	{
		free(g_buffer.s);
		g_buffer.s = NULL;
		exit_readline();
	}
	else
	{
		buff_del_next();
	}
}

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
	dprintf(3, "%-10s = %-6d\n", "r->start", r->start);
	dprintf(3, "%-10s = %-6d\n", "r->bufsize", r->bufsize);
	dprintf(3, "%-10s = %#.4x\n", "r->action", r->action);
	dprintf(3, "%-10s = %s\n", "r->buf", r->buf);
}

void	buff_revert_write(t_record *r)
{
	shift_cursor(g_cubuf, r->start);
	memmove(g_buffer.s + r->start + r->bufsize,
			g_buffer.s + r->start,
			r->bufsize);
	memcpy(g_buffer.s + r->start,
		   r->buf,
		   r->bufsize);
	g_buffer.len += r->bufsize;
	buff_refresh(r->start,
				 g_buffer.s + r->start,
				 g_buffer.len - r->start);
	g_cubuf = r->start + r->bufsize;
	shift_cursor(g_buffer.len, g_cubuf);
	dprintf(3, "buff_revert_write\n");
}

void	buff_revert_delete(t_record *r)
{
	memmove(g_buffer.s + r->start + r->bufsize,
			g_buffer.s + r->start,
			g_buffer.len - r->start);
	memset(g_buffer.s + g_buffer.len + r->bufsize,
		   ' ',
		   -(r->bufsize));
	shift_cursor(g_cubuf, r->start + r->bufsize);
	g_cubuf = r->start + r->bufsize;
	buff_refresh(g_cubuf,
				 g_buffer.s + g_cubuf,
				 g_buffer.len - g_cubuf);
	shift_cursor(g_buffer.len, g_cubuf);
	g_buffer.len += r->bufsize;
	dprintf(3, "buff_revert_delete\n");
}

void	buff_revert_overwrite(t_record *r)
{
	memcpy(g_buffer.s + r->start, r->buf,
		   r->bufsize);
	shift_cursor(g_cubuf, r->start);
	buff_refresh(r->start, g_buffer.s + r->start, r->bufsize);
	shift_cursor(r->start + r->bufsize, r->start);
	g_cubuf = r->start;
	dprintf(3, "buff_revert_overwrite\n");
}

void	buff_revert(void)
{
	static void (*act[3])() = {
		buff_revert_write,
		buff_revert_delete,
		buff_revert_overwrite
	};
	t_record	*r;

	if (!g_record)
		return ;
	r = (t_record *)g_record->data;
	act[r->action](r);
	free(r->buf);
	pop_lst__(&g_record, free);
}
