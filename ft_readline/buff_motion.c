/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   buff_motion.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: root <marvin@42.fr>                        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2017/07/17 10:51:14 by root              #+#    #+#             */
/*   Updated: 2017/09/20 19:11:26 by jye              ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft_readline.h"
#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <curses.h>
#include <term.h>
#include <dirent.h>

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
