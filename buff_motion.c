/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   buff_motion.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: root <marvin@42.fr>                        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2017/07/17 10:51:14 by root              #+#    #+#             */
/*   Updated: 2017/07/27 12:14:47 by root             ###   ########.fr       */
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
