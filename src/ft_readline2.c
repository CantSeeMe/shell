/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_readline2.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jye <jye@student.42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2017/09/20 19:34:38 by jye               #+#    #+#             */
/*   Updated: 2017/09/25 21:31:29 by root             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft_readline.h"
#include "libft.h"
#include "ft_printf.h"

#include <sys/ioctl.h>
#include <stdlib.h>
#include <unistd.h>
#include <curses.h>
#include <term.h>

int		update_winsize(void)
{
	struct winsize w;

	if ((ioctl(STDIN_FILENO, TIOCGWINSZ, &w)))
		return (1);
	g_winsize.col = w.ws_col;
	g_winsize.row = w.ws_row;
	return (0);
}

void	reload_line(int s)
{
	int	row;

	(void)s;
	update_winsize();
	row = (g_psize + g_buffer.cu) / g_winsize.col;
	while (row--)
		tputs(tgetstr("up", 0), 0, putchar_);
	tputs(tgetstr("cr", 0), 0, putchar_);
	tputs(tgetstr("cd", 0), 0, putchar_);
	write(STDERR_FILENO, g_prompt, g_psize);
	buff_refresh(0, g_buffer.s, g_buffer.len);
	shift_cursor(g_buffer.len, g_buffer.cu);
}

void	exit_readline(void)
{
	t_chronicle	*chro;

	buff_newline();
	g_buffer.msize = 0;
	while (g_record)
	{
		free(((t_record *)g_record->data)->buf);
		pop_lst__(&g_record, free);
	}
	chro = (t_chronicle *)g_chroncur->data;
	chro->record = 0;
	chro = g_chronicle->data;
	if (g_buffer.s)
		g_buffer.s[g_buffer.len] = 0;
	if (g_buffer.len)
		chro->s = ft_strdup(g_buffer.s);
}

void	reset_readline(void)
{
	buff_newline();
	g_buffer.msize = 0;
	while (g_record)
	{
		free(((t_record *)g_record->data)->buf);
		pop_lst__(&g_record, free);
	}
	if (g_chroncur != g_chronicle)
		free(((t_chronicle *)g_chronicle->data)->cur.s);
	free(g_buffer.s);
	g_buffer.s = (char *)-1;
	pop_lst__(&g_chronicle, free);
}
