/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   buff_motion8.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jye <jye@student.42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2017/09/20 19:10:28 by jye               #+#    #+#             */
/*   Updated: 2017/09/30 21:26:31 by jye              ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft_readline.h"
#include "libft.h"
#include "ft_printf.h"

#include <stdlib.h>
#include <curses.h>
#include <term.h>
#include <dirent.h>

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
	ft_memcpy(g_yank, r->buf, r->bufsize);
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
	ft_memcpy(g_yank, r->buf, r->bufsize);
	ft_memcpy(g_yank + r->bufsize, tmp, g_yanksize);
	g_yanksize += r->bufsize;
	free(tmp);
}

void	buff_yankin(void)
{
	static void	(*prev_action)();

	if (g_last_action == buff_kill_prev || g_last_action == buff_kill_next ||
		g_last_action == buff_del_word)
	{
		if (prev_action == buff_del_word && g_last_action == buff_del_word)
			buff_yank_keep();
		else
			buff_yank_reset();
	}
	prev_action = g_last_action;
}

void	buff_yankout(void)
{
	if (g_yanksize == 0)
		return ;
	buff_insert(g_yank, g_yanksize);
}
