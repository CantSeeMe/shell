/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   prompt.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: root <marvin@42.fr>                        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2017/07/10 15:04:56 by root              #+#    #+#             */
/*   Updated: 2017/07/27 13:52:49 by root             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <termios.h>
#include <curses.h>
#include <term.h>
#include <unistd.h>
#include <stdlib.h>
#include <stddef.h>
#include <stdint.h>
#include <string.h>
#include <signal.h>
#include <sys/ioctl.h>

#include "prompt.h"
#include "lst.h"

int		putchar_(int c)
{
	return (write(STDERR_FILENO, &c, 1));
}

/******** termios part **********/

int		set_manual_ttymode(void)
{
	struct termios termios;

	tcgetattr(STDIN_FILENO, &termios);
	g_otermios = termios;
	termios.c_oflag &= ~OPOST;
	termios.c_lflag &= ~(ECHO | ECHONL | ICANON | ISIG | IEXTEN);
	return (tcsetattr(STDIN_FILENO, TCSANOW, &termios));
}

int		revert_manual_ttymode(void)
{
	return (tcsetattr(STDIN_FILENO, TCSANOW, &g_otermios));
}

/********** cursor **************/

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
		tputs(tgetstr("up", 0), 0, putchar_);

	while (col > 0)
	{
		tputs(tgetstr("nd", 0), 0, putchar_);
		col--;
	}
	while (col++ < 0)
		tputs(tgetstr("le", 0), 0, putchar_);
}

/********************************/

/*********** screen *************/

int		update_screen_size(void)
{
	struct winsize w;

	ioctl(STDIN_FILENO, TIOCGWINSZ, &w);
	g_winsize.col = w.ws_col;
	g_winsize.row = w.ws_row;
	return (0);
}

/********************************/

/*********** buffer *************/

int		malloc_buffer(size_t s)
{
	if ((g_buffer.s = malloc(s)) == 0)
		return (1);
	g_buffer.msize = s;
	return (0);
}

int		realloc_buffer(size_t s)
{
	char	*t;

	t = g_buffer.s;
	if (malloc_buffer(s))
	{
		g_buffer.s = t;
		return (1);
	}
	memcpy(g_buffer.s, t, g_buffer.len);
	free(t);
	return (0);
}

void	refresh_buffer(int cursor, char *s, ssize_t slen)
{
	static char	q[] = {0, ' ', '\r'};
	int			col;

	col = (cursor + g_psize) % g_winsize.col;
	while (slen--)
	{
		q[0] = *s++;
		++col;
		if (col >= g_winsize.col)
		{
			write(STDERR_FILENO, q, 3);
			col = 0;
		}
		else
		{
			write(STDERR_FILENO, q, 1);
		}
	}
}

/********************************/

/********keyboard event**********/

void	default_event(uint64_t c, int r)
{
	if (g_buffer.len + r >= g_buffer.msize)
	{
		if (realloc_buffer(g_buffer.msize * 2))
			return ;
	}
	memcpy(g_buffer.s + g_cubuf + r,
		   g_buffer.s + g_cubuf,
		   g_buffer.len - g_cubuf);
	memcpy(g_buffer.s + g_cubuf, &c, r);
	g_buffer.len += r;
	refresh_buffer(g_cubuf, g_buffer.s + g_cubuf, g_buffer.len - g_cubuf);
	g_cubuf += r;
	shift_cursor(g_buffer.len, g_cubuf);
}

void	place_holder(void)
{
	return ;
}

void	ctrl_event(uint64_t c)
{
	static void (*f[])() = {
		buff_head, buff_prev, place_holder, place_holder, buff_end,
		buff_next, place_holder, place_holder, place_holder, place_holder,
		place_holder, place_holder, place_holder, place_holder, place_holder,
		place_holder, place_holder, place_holder, place_holder, place_holder,
		place_holder, place_holder, place_holder, place_holder, place_holder,
		place_holder, place_holder, place_holder, place_holder, place_holder,
		place_holder, place_holder};

	f[c - 1]();
}

/*
**f forward word
**b backward word
**r reset history and prompt
**u upper case
**l lower case
**c forward word upper first letter
*/

void	meta_event(void)
{
	uint64_t	c;

	c = 0;
	read(STDIN_FILENO, &c, sizeof(c));
	if (c == 'f')
		buff_next_word();
	else if (c == 'b')
		buff_prev_word();
	else if (c == 'u')
	{}
	else if (c == 'l')
	{}
	else if (c == 'r')
	{}
	else if (c == 'c')
	{}
}

void	special_event(uint64_t c)
{
	static void (*special1[8])() = {
		place_holder, place_holder, buff_next, buff_prev,
		place_holder, buff_head, place_holder, buff_end};
	static void (*special2[4])() = {
		place_holder, place_holder, buff_next_word, buff_prev_word};
	static void (*special3[4])() = {
		place_holder, place_holder, place_holder, place_holder};

	c = c >> 16;
	if (c >= 0x41 && c <= 0x48)
	{
		special1[c - 0x41]();
	}
	else if (IS_CTRL_ARROW(c))
	{
		special2[(c >> 24) - 0x41]();
	}
	else if ((c & 0x7e30) == 0x7e30 &&
			((c & 0xff) > 2 && (c & 0xff) <= 6))
	{
		special3[(c ^ 0x7e30) - 0x3]();
	}
}

void	backspace_event(void)
{
	
}

void	keyboard_event(uint64_t	c, int r)
{
	if (IS_BACKSPACE_KEYCODE(c))
		backspace_event();
	else if (IS_SPECIAL_KEYCODE(c))
		special_event(c);
	else if (IS_META_MODIFIER(c))
		meta_event();
	else if (IS_CTRL_MODIFIER(c))
		ctrl_event(c);
	else
		default_event(c, r);
}

/********************************/

void	update_tty(int s)
{
	int	row;

	(void)s;
	update_screen_size();
	row = (g_psize + g_cubuf) / g_winsize.col;
	while (row--)
		tputs(tgetstr("up", 0), 0, putchar_);
	tputs(tgetstr("cr", 0), 0, putchar_);
	tputs(tgetstr("cd", 0), 0, putchar_);
//write(STDERR_FILENO, g_prompt, strlen(g_prompt));
	refresh_buffer(0, g_buffer.s, g_buffer.len);
	shift_cursor(g_buffer.len, g_cubuf);
}

char	*prompt_user(char *p, size_t psize)
{
	uint64_t	c;
	int			r;

	if (g_buffer.s == 0)
		malloc_buffer(DEFAULT_BUFFER_SIZE);
	if (set_manual_ttymode())
	{
		// tcsetattr failed, setting back original termios attribute
		tcsetattr(STDIN_FILENO, TCSADRAIN, &g_otermios);
		return (0);
	}
	tgetent(getenv("TERM"), NULL);
	update_screen_size();
//	signal(SIGINT, );
	while (42)
	{
		signal(SIGWINCH, update_tty);
		c = 0;
		r = read(STDIN_FILENO, &c, sizeof(c));
		keyboard_event(c, r);
//		dprintf(1, "%lx , %d\n", c, r);
		if (c == 'q')
			break ;
	}
	revert_manual_ttymode();
	return (g_buffer.s);
}

int		main(int ac, char **av, char **envp)
{
	prompt_user("", 0);
}
