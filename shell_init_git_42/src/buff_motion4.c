/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   buff_motion4.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jye <jye@student.42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2017/09/20 19:07:41 by jye               #+#    #+#             */
/*   Updated: 2017/09/24 11:36:18 by jye              ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft_readline.h"
#include "libft.h"

#include <stdlib.h>
#include <curses.h>
#include <term.h>
#include <dirent.h>

void	buff_lowtoup(void)
{
	char	*s;
	int		i;
	int		diff;

	s = g_buffer.s + g_buffer.cu;
	g_buffer.s[g_buffer.len] = 0;
	diff = 0;
	while (!ft_strchr(WORD_ANCHAR, s[diff]))
		diff++;
	i = diff;
	while (s[diff] && ft_strchr(WORD_ANCHAR, s[diff]))
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
	while (!ft_strchr(WORD_ANCHAR, s[diff]))
		diff++;
	i = diff;
	while (s[diff] && ft_strchr(WORD_ANCHAR, s[diff]))
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
	while (!ft_strchr(WORD_ANCHAR, s[diff]))
		diff++;
	i = diff;
	while (s[diff] && ft_strchr(WORD_ANCHAR, s[diff]))
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
