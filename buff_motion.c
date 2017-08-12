/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   buff_motion.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: root <marvin@42.fr>                        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2017/07/17 10:51:14 by root              #+#    #+#             */
/*   Updated: 2017/07/31 11:52:13 by root             ###   ########.fr       */
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
	char	*ptr;

	s = g_buffer.s;
	s[g_buffer.len] = 0;
	i = g_cubuf;
	while (!(ptr = strchr(WORD_ANCHAR, i[s])))
	{
		if (i == g_buffer.len)
			break ;
		i++;
	}
	while ((ptr = strchr(WORD_ANCHAR, i[s])))
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
	char	*ptr;

	s = g_buffer.s;
	i = g_cubuf - (g_cubuf > 0);
	while (!(ptr = strchr(WORD_ANCHAR, i[s])))
	{
		if (i <= 0)
			break ;
		i--;
	}
	while ((ptr = strchr(WORD_ANCHAR, i[s])))
	{
		if (i <= 0)
			break ;
		i--;
	}
	shift_cursor(g_cubuf, i + !ptr);
	g_cubuf = i + !ptr;
}

void	buff_del_prev(void)
{
	if (g_cubuf == 0)
		return ;
	g_cubuf -= 1;
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
	int		diff;

	s = g_buffer.s + g_cubuf;
	g_buffer.s[g_buffer.len] = 0;
	while (!strchr(WORD_ANCHAR, *s))
		++s;
	while (*s && strchr(WORD_ANCHAR, *s))
	{
		if (*s >= 0x60 && *s <= 0x7a)
			*s -= 0x20;
		++s;
	}
	diff = s - (g_buffer.s + g_cubuf);
	buff_refresh(g_cubuf, g_buffer.s + g_cubuf, diff);
	g_cubuf += diff;
}
	
void	buff_uptolow(void)
{
	char	*s;
	int		diff;

	s = g_buffer.s + g_cubuf;
	g_buffer.s[g_buffer.len] = 0;
	while (!strchr(WORD_ANCHAR, *s))
		++s;
	while (*s && strchr(WORD_ANCHAR, *s))
	{
		if (*s >= 0x40 && *s <= 0x5a)
			*s += 0x20;
		++s;
	}
	diff = s - (g_buffer.s + g_cubuf);
	buff_refresh(g_cubuf, g_buffer.s + g_cubuf, diff);
	g_cubuf += diff;
}

void	buff_capitalize(void)
{
	char	*s;
	int		l;
	int		diff;

	g_buffer.s[g_buffer.len] = 0;
	s = g_buffer.s + g_cubuf;
	l = 0;
	while (!strchr(WORD_ANCHAR, *s))
		++s;
	while (*s && strchr(WORD_ANCHAR, *s))
	{
		if (!l && *s >= 0x60 && *s <= 0x7a)
			*s -= 0x20;
		if (l && *s >= 0x40 && *s <= 0x5a)
			*s += 0x20;
		l = 1;
		++s;
	}
	diff = s - (g_buffer.s + g_cubuf);
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

void	buff_record_write(int start, int end, int action)
{
	t_record	*r;

}

void	buff_record_delete(int start, int end)
{
	t_record	*r;
}

int		buff_record(int end, int action)
{
	static size_t	start;
	static int		last_action;
	static void		(*act[2])() = {};

	if (last_action != action)
	{
		if (last_action != RL_ACTION_MOVE)
			act[last_action](start, end);
		last_action = action;
		start = end;
	}
}

void	buff_revert_line(void)
{

}
