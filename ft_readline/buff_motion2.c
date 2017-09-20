/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   buff_motion2.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jye <jye@student.42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2017/09/20 19:06:12 by jye               #+#    #+#             */
/*   Updated: 2017/09/20 19:06:31 by jye              ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft_readline.h"
#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <curses.h>
#include <term.h>
#include <dirent.h>

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
	write(2, g_prompt, strlen(g_prompt));
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
	chro->record = 0;
}
