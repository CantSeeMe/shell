/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cursor.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: root <marvin@42.fr>                        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2017/07/30 08:43:16 by root              #+#    #+#             */
/*   Updated: 2017/09/20 18:07:49 by jye              ###   ########.fr       */
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
		row--;
	}
	while (row++ < 0)
	{
		tputs(tgetstr("up", 0), 0, putchar_);
	}
	while (col > 0)
	{
		tputs(tgetstr("nd", 0), 0, putchar_);
		col--;
	}
	while (col++ < 0)
	{
		tputs(tgetstr("le", 0), 0, putchar_);
	}
}
