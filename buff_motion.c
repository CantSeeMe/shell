/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   buff_motion.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: root <marvin@42.fr>                        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2017/07/17 10:51:14 by root              #+#    #+#             */
/*   Updated: 2017/08/27 21:46:16 by root             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#define WORD_UCHAR	"ABCDEFGHIJKLMNOPQRSTUVWXYZ"
#define WORD_LCHAR	"abcdefghijklmnopqrstuvwxyz"
#define WORD_NCHAR	"1234567890"

#define WORD_ANCHAR	WORD_UCHAR WORD_LCHAR WORD_NCHAR

#include "ft_readline.h"
#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <curses.h>
#include <term.h>

void	buff_head(void)
{
	shift_cursor(g_buffer.cu, 0);
	g_buffer.cu = 0;
}

void	buff_end(void)
{
	shift_cursor(g_buffer.cu, g_buffer.len);
	g_buffer.cu = g_buffer.len;
}

void	buff_prev(void)
{
	shift_cursor(g_buffer.cu, g_buffer.cu - (g_buffer.cu > 0));
	g_buffer.cu -= g_buffer.cu > 0;
}

void	buff_next(void)
{
	shift_cursor(g_buffer.cu, g_buffer.cu + (g_buffer.cu < g_buffer.len));
	g_buffer.cu += g_buffer.cu < g_buffer.len;
}

void	buff_next_word(void)
{
	char	*s;
	size_t	i;

	s = g_buffer.s;
	s[g_buffer.len] = 0;
	i = g_buffer.cu;
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
	shift_cursor(g_buffer.cu, i);
	g_buffer.cu = i;
}

void	buff_prev_word(void)
{
	char	*s;
	int		i;

	s = g_buffer.s;
	if ((i = g_buffer.cu - (g_buffer.cu > 0)) == -1)
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
	shift_cursor(g_buffer.cu, i + (i > 0));
	g_buffer.cu = i + (i > 0);
}

void	buff_del_prev(void)
{
	if (g_buffer.cu == 0)
		return ;
	g_buffer.cu -= 1;
	buff_record(g_buffer.cu, 1, RL_ACTION_WRITE);
	memcpy(g_buffer.s + g_buffer.cu,
		   g_buffer.s + g_buffer.cu + 1,
		   g_buffer.len - g_buffer.cu);
	shift_cursor(g_buffer.cu + 1, g_buffer.cu);
	g_buffer.len -= 1;
	g_buffer.s[g_buffer.len] = ' ';
	buff_refresh(g_buffer.cu,
				 g_buffer.s + g_buffer.cu,
				 g_buffer.len - g_buffer.cu + 1);
	shift_cursor(g_buffer.len + 1, g_buffer.cu);
}

void	buff_del_next(void)
{
	if (g_buffer.cu == g_buffer.len)
		return ;
	buff_record(g_buffer.cu, 1, RL_ACTION_WRITE | RL_CURSOR_KEEP);
	memcpy(g_buffer.s + g_buffer.cu,
		   g_buffer.s + g_buffer.cu + 1,
		   g_buffer.len - g_buffer.cu);
	g_buffer.len -= 1;
	g_buffer.s[g_buffer.len] = ' ';
	buff_refresh(g_buffer.cu,
				 g_buffer.s + g_buffer.cu,
				 g_buffer.len - g_buffer.cu + 1);
	shift_cursor(g_buffer.len + 1, g_buffer.cu);
}

void	buff_del_word(void)
{
	int		cubuf;
	int		diff;

	cubuf = g_buffer.cu;
	buff_prev_word();
	diff = cubuf - g_buffer.cu;
	buff_record(g_buffer.cu, diff, RL_ACTION_WRITE);
	g_buffer.len -= diff;
	memcpy(g_buffer.s + g_buffer.cu,
		   g_buffer.s + cubuf,
		   g_buffer.len - g_buffer.cu);
	memset(g_buffer.s + g_buffer.len, ' ', diff);
	buff_refresh(g_buffer.cu, g_buffer.s + g_buffer.cu, g_buffer.len - g_buffer.cu + diff);
	shift_cursor(g_buffer.len + diff, g_buffer.cu);
}

void	buff_kill_next(void)
{
	int		diff;

	diff = g_buffer.len - g_buffer.cu;
	buff_record(g_buffer.cu, diff, RL_ACTION_WRITE | RL_CURSOR_KEEP);
	memset(g_buffer.s + g_buffer.cu, ' ', diff);
	buff_refresh(g_buffer.cu, g_buffer.s + g_buffer.cu, diff);
	shift_cursor(g_buffer.len, g_buffer.cu);
	g_buffer.len = g_buffer.cu;
}

void	buff_kill_prev(void)
{
	int		diff;

	diff = g_buffer.len - g_buffer.cu;
	buff_record(0, g_buffer.cu, RL_ACTION_WRITE);
	memmove(g_buffer.s,
			g_buffer.s + g_buffer.cu,
			diff);
	memset(g_buffer.s + diff, ' ', g_buffer.cu);
	shift_cursor(g_buffer.cu, 0);
	g_buffer.cu = 0;
	buff_refresh(0, g_buffer.s, g_buffer.len);
	shift_cursor(g_buffer.len, 0);
	g_buffer.len = diff;
}

void	buff_clear_content(void)
{
	tputs(tgetstr("cl", 0), 0, putchar_);
	// print prompt
	buff_refresh(0, g_buffer.s, g_buffer.len);
	shift_cursor(g_buffer.len, g_buffer.cu);
}

void	buff_reset_state(void)
{
	t_chronicle	*chro;
	size_t		l;

	chro = (t_chronicle *)g_chroncur->data;
	shift_cursor(g_buffer.cu, 0);
	if (chro->s)
	{
		l = strlen(chro->s);
		memcpy(g_buffer.s, chro->s, l);
	}
	else
		l = 0;
	memset(g_buffer.s + l, ' ', g_buffer.len - l);
	buff_refresh(0, g_buffer.s, g_buffer.len);
	shift_cursor(g_buffer.len, l);
	g_buffer.len = l;
	g_buffer.cu = l;
	while (g_record)
	{
		free(((t_record *)g_record->data)->buf);
		pop_lst__(&g_record, free);
	}
}

/*****************************/
/*****************************/
/*****************************/

void	buff_autocomplete(void)
{
	
}

void	buff_lowtoup(void)
{
	char	*s;
	int		i;
	int		diff;

	s = g_buffer.s + g_buffer.cu;
	g_buffer.s[g_buffer.len] = 0;
	diff = 0;
	while (!strchr(WORD_ANCHAR, s[diff]))
		diff++;
	i = diff;
	while (s[diff] && strchr(WORD_ANCHAR, s[diff]))
		diff++;
	buff_record(g_buffer.cu, diff, RL_ACTION_OVERWRITE | RL_CURSOR_KEEP);
	while (i < diff)
	{
		if (s[i] >= 0x60 && s[i] <= 0x7a)
			s[i] -= 0x20;
		++i;
	}
	buff_refresh(g_buffer.cu, g_buffer.s + g_buffer.cu, diff);
	g_buffer.cu += diff;
}
	
void	buff_uptolow(void)
{
	char	*s;
	int		diff;
	int		i;

	s = g_buffer.s + g_buffer.cu;
	g_buffer.s[g_buffer.len] = 0;
	diff = 0;
	while (!strchr(WORD_ANCHAR, s[diff]))
		diff++;
	i = diff;
	while (s[diff] && strchr(WORD_ANCHAR, s[diff]))
		diff++;
	buff_record(g_buffer.cu, diff, RL_ACTION_OVERWRITE | RL_CURSOR_KEEP);
	while (i < diff)
	{
		if (s[i] >= 0x40 && s[i] <= 0x5a)
			s[i] += 0x20;
		i++;
	}
	buff_refresh(g_buffer.cu, g_buffer.s + g_buffer.cu, diff);
	g_buffer.cu += diff;
}

void	buff_capitalize(void)
{
	char	*s;
	int		diff;
	int		i;

	g_buffer.s[g_buffer.len] = 0;
	s = g_buffer.s + g_buffer.cu;
	diff = 0;
	while (!strchr(WORD_ANCHAR, s[diff]))
		diff++;
	i = diff;
	while (s[diff] && strchr(WORD_ANCHAR, s[diff]))
		diff++;
	buff_record(g_buffer.cu, diff, RL_ACTION_OVERWRITE | RL_CURSOR_KEEP);
	if (s[i] >= 0x60 && s[i] <= 0x7a)
		s[i] -= 0x20;
	while (++i < diff)
		if (s[i] >= 0x40 && s[i] <= 0x5a)
			s[i] += 0x20;
	buff_refresh(g_buffer.cu, g_buffer.s + g_buffer.cu, diff);
	g_buffer.cu += diff;
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
	diff -= (g_psize + g_buffer.cu) / g_winsize.col;
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

void	buff_insert(void *c, int r)
{
	if (g_buffer.len + r >= g_buffer.msize)
	{
		if (buff_realloc(g_buffer.msize + DEFAULT_BUFFER_SIZE))
			return ;
	}
	memcpy(g_buffer.s + g_buffer.cu + r,
		   g_buffer.s + g_buffer.cu,
		   g_buffer.len - g_buffer.cu);
	memcpy(g_buffer.s + g_buffer.cu, c, r);
	g_buffer.len += r;
	buff_refresh(g_buffer.cu, g_buffer.s + g_buffer.cu, g_buffer.len - g_buffer.cu);
	g_buffer.cu += r;
	buff_record(g_buffer.cu, -r, RL_ACTION_DELETE);
	shift_cursor(g_buffer.len, g_buffer.cu);
}

/********************************/
/********************************/
/********************************/

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
	memset(g_buffer.s + g_buffer.len + r->bufsize,
		   ' ',
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
	memcpy(g_buffer.s + r->start, r->buf,
		   r->bufsize);
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

/**********************/
/**********************/
/**********************/

void	buff_yank_reset(void)
{
	t_record	*r;

	free(g_yank);
	r = g_record->data;
	if ((g_yank = malloc(r->bufsize)) == 0)
	{
		g_yank = 0;
		g_yanksize = 0;
		return ;
	}
	memcpy(g_yank, r->buf, r->bufsize);
	g_yanksize = r->bufsize;
}

void	buff_yank_keep(void)
{
	t_record	*r;
	char		*tmp;

	if (g_yank == 0)
	{
		buff_yank_reset();
		return ;
	}
	r = g_record->data;
	tmp = g_yank;
	if ((g_yank = malloc(g_yanksize + r->bufsize)) == 0)
	{
		g_yank = tmp;
		return ;
	}
	memcpy(g_yank, r->buf, r->bufsize);
	memcpy(g_yank + r->bufsize, tmp, g_yanksize);
	g_yanksize += r->bufsize;
	free(tmp);
}

void	buff_yankin(void)
{
	static void	(*prev_action)();

	if (last_action == buff_kill_prev || last_action == buff_kill_next ||
		last_action == buff_del_word)
	{
		if (prev_action == buff_del_word && last_action == buff_del_word)
			buff_yank_keep();
		else
			buff_yank_reset();
	}
	prev_action = last_action;
}

void	buff_yankout(void)
{
	if (g_yanksize == 0)
		return ;
	buff_insert(g_yank, g_yanksize);
}

/*******************************/
/*******************************/
/*******************************/
void	buff_chronicow(char *s, size_t len)
{
	shift_cursor(g_buffer.cu, 0);
	memcpy(g_buffer.s, s, len);
	if (g_buffer.len > len)
		memset(g_buffer.s + len, ' ', g_buffer.len - len);
    g_buffer.cu = len;
	buff_refresh(0, g_buffer.s, g_buffer.len > len ? g_buffer.len : len);
	if (g_buffer.len > len)
		shift_cursor(g_buffer.len, len);
	g_buffer.len = len;
}

void	buff_chronicup(void)
{
	t_chronicle	*chro;

	if (g_chroncur->next == NULL)
		return ;
	chro = g_chroncur->data;
	chro->cur = ((t_buff){.s = 0, .msize = 0, .len = g_buffer.len, .cu = 0});
	g_buffer.s[g_buffer.len] = 0;
	chro->cur.len = g_buffer.len;
	if (g_buffer.len)
		chro->cur.s = strdup(g_buffer.s);
	chro->record = g_record;
	g_chroncur = g_chroncur->next;
	chro = g_chroncur->data;
	g_record = chro->record;
	if (chro->cur.s != 0)
	{
		buff_chronicow(chro->cur.s, chro->cur.len);
		free(chro->cur.s);
		chro->cur.s = 0;
	}
	else if (chro->s)
		buff_chronicow(chro->s, strlen(chro->s));
	else
		buff_chronicow("", 0);
}

void	buff_chronicdown(void)
{
	t_chronicle	*chro;

	if (g_chroncur->prev == NULL)
		return ;
	chro = g_chroncur->data;
	chro->cur = ((t_buff){.s = 0, .msize = 0, .len = g_buffer.len, .cu = 0});
	g_buffer.s[g_buffer.len] = 0;
	chro->cur.len = g_buffer.len;
	if (g_buffer.len)
		chro->cur.s = strdup(g_buffer.s);
	chro->record = g_record;
	g_chroncur = g_chroncur->prev;
	chro = g_chroncur->data;
	g_record = chro->record;
	if (chro->cur.s != 0)
	{
		buff_chronicow(chro->cur.s, chro->cur.len);
		free(chro->cur.s);
		chro->cur.s = 0;
	}
	else if (chro->s)
		buff_chronicow(chro->s, strlen(chro->s));
	else
		buff_chronicow("", 0);
}
