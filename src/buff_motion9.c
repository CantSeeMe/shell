/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   buff_motion.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: root <marvin@42.fr>                        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2017/07/17 10:51:14 by root              #+#    #+#             */
/*   Updated: 2017/09/25 22:13:43 by root             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft_readline.h"
#include "libft.h"

#include <stdlib.h>
#include <curses.h>
#include <term.h>
#include <dirent.h>

void	buff_chronicow(char *s, size_t len)
{
	char	*tmp;

	if (len > g_buffer.msize)
	{
		tmp = g_buffer.s;
		if (buff_malloc((len & ~DEFAULT_BUFFER_SIZE) + DEFAULT_BUFFER_SIZE))
		{
			g_buffer.s = tmp;
			return ;
		}
		free(tmp);
	}
	shift_cursor(g_buffer.cu, 0);
	ft_memcpy(g_buffer.s, s, len);
	if (g_buffer.len > len)
		ft_memset(g_buffer.s + len, ' ', g_buffer.len - len);
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
		chro->cur.s = ft_strdup(g_buffer.s);
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
		buff_chronicow(chro->s, ft_strlen(chro->s));
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
		chro->cur.s = ft_strdup(g_buffer.s);
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
		buff_chronicow(chro->s, ft_strlen(chro->s));
	else
		buff_chronicow("", 0);
}
