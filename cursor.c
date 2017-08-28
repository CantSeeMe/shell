/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cursor.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: root <marvin@42.fr>                        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2017/07/30 08:43:16 by root              #+#    #+#             */
/*   Updated: 2017/08/27 21:41:59 by root             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft_readline.h"
#include <curses.h>
#include <term.h>

void	shift_cursor(int current, int target)
{
	int	col;
	int	row;

	col = ((g_psize + target) % g_winsize.col) -
		  ((g_psize + current) % g_winsize.col);
	row = ((g_psize + target) / g_winsize.col) -
		  ((g_psize + current) / g_winsize.col);
	while (row > 0)
	{
		tputs(tgetstr("do", 0), 0, putchar_);
//		dprintf(3, "do ");
		row--;
	}
	while (row++ < 0)
	{
		tputs(tgetstr("up", 0), 0, putchar_);
//		dprintf(3, "up ");
	}
	while (col > 0)
	{
		tputs(tgetstr("nd", 0), 0, putchar_);
//		dprintf(3, "nd ");
		col--;
	}
	while (col++ < 0)
	{
//		dprintf(3, "le ");
		tputs(tgetstr("le", 0), 0, putchar_);
	}
//	dprintf(3, "\n");
}
