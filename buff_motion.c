/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   buff_motion.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: root <marvin@42.fr>                        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2017/07/17 10:51:14 by root              #+#    #+#             */
/*   Updated: 2017/07/29 20:40:34 by root             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#define WORD_UCHAR	"ABCDEFGHIJKLMNOPQRSTUVWXYZ"
#define WORD_LCHAR	"abcdefghijklmnopqrstuvwxyz"
#define WORD_NCHAR	"1234567890"

#define WORD_ANCHAR	WORD_UCHAR WORD_LCHAR WORD_NCHAR

#include "prompt.h"
#include <string.h>
#include <stdio.h>

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
	refresh_buffer(g_cubuf,
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
	refresh_buffer(g_cubuf,
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
	refresh_buffer(g_cubuf, g_buffer.s + g_cubuf, g_buffer.len - g_cubuf + diff);
	shift_cursor(g_buffer.len + diff, g_cubuf);
}

void	buff_new_line(void)
{
	int		diff;

	diff = (g_psize + g_buffer.len) / g_winsize.col -
		(g_psize + g_cubuf) / g_winsize.col;
	while (diff--)
	{
		write(STDERR_FILENO, '\n', 1);
	}
	write(STDERR_FILENO, '\r', 1);
}

int		init_readline(void)
{

}

void	exit_readline(void)
{
	buff_new_line();
}
