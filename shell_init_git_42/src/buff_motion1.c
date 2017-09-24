/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   buff_motion1.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jye <jye@student.42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2017/09/20 19:05:22 by jye               #+#    #+#             */
/*   Updated: 2017/09/24 11:35:33 by jye              ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft_readline.h"
#include "libft.h"

#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <curses.h>
#include <term.h>
#include <dirent.h>

void	buff_next_word(void)
{
	char	*s;
	size_t	i;

	s = g_buffer.s;
	s[g_buffer.len] = 0;
	i = g_buffer.cu;
	while (!ft_strchr(WORD_ANCHAR, i[s]))
	{
		if (i == g_buffer.len)
			break ;
		i++;
	}
	while (ft_strchr(WORD_ANCHAR, i[s]))
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
	while (!ft_strchr(WORD_ANCHAR, i[s]))
	{
		if (i <= 0)
			break ;
		i--;
	}
	while (ft_strchr(WORD_ANCHAR, i[s]))
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
	ft_memcpy(g_buffer.s + g_buffer.cu,
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
	ft_memcpy(g_buffer.s + g_buffer.cu,
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
	ft_memcpy(g_buffer.s + g_buffer.cu,
			g_buffer.s + cubuf,
			g_buffer.len - g_buffer.cu);
	ft_memset(g_buffer.s + g_buffer.len, ' ', diff);
	buff_refresh(g_buffer.cu, g_buffer.s + g_buffer.cu,
					g_buffer.len - g_buffer.cu + diff);
	shift_cursor(g_buffer.len + diff, g_buffer.cu);
}
