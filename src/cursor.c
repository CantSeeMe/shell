/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cursor.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: root <marvin@42.fr>                        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2017/07/30 08:43:16 by root              #+#    #+#             */
/*   Updated: 2017/11/14 03:13:08 by jye              ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft_readline.h"
#include "ft_printf.h"
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
		ft_dprintf(3, "do\n");
		tputs(tgetstr("do", 0), 0, putchar_);
		row--;
	}
	while (row++ < 0)
	{
		ft_dprintf(3, "up\n");
		tputs(tgetstr("up", 0), 0, putchar_);
	}
	while (col > 0)
	{
		ft_dprintf(3, "nd\n");
		tputs(tgetstr("nd", 0), 0, putchar_);
		col--;
	}
	while (col++ < 0)
	{
		ft_dprintf(3, "le\n");
		tputs(tgetstr("le", 0), 0, putchar_);
	}
}
