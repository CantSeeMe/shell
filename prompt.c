/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   prompt.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: root <marvin@42.fr>                        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2017/07/10 15:04:56 by root              #+#    #+#             */
/*   Updated: 2017/07/14 10:07:44 by root             ###   ########.fr       */
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
	termios.c_lflag &= ~(ISIG | ECHO | ECHOE | ICANON);
	return (tcsetattr(STDIN_FILENO, TCSADRAIN, &termios));

}

int		revert_manual_ttymode(void)
{
	return (tcsetattr(STDIN_FILENO, TCSADRAIN, &g_otermios));
}

/********** cursor **************/

void	move_cursor(ssize_t s)
{
	g_cursor.x += s;
	if (g_cursor.x < 0)
	{
		while (g_cursor.x < 0)
		{
			g_cursor.y -= 1;
			g_cursor.x = g_ttysize.x + g_cursor.x;
		}
	}
	else if (g_cursor.x >= g_ttysize.x)
	{
		while (g_cursor.x >= g_ttysize.x)
		{
			g_cursor.y += 1;
			if (g_cursor.y >= g_ttysize.y)
			{
				g_pactual.y -= 1;
				g_cursor.y -= 1;
			}
			g_cursor.x = g_cursor.x - g_ttysize.x;
		}
	}
}

void	set_cursor(int rows, int cols)
{
	tputs(tparm(tgetstr("cm", 0), rows, cols), 1, putchar_);
}

void	update_cursor(void)
{
	char	s[15];

	write(STDERR_FILENO, "\e[6n" , 4);
	s[read(STDIN_FILENO, s, 15)] = 0;
	g_cursor.x = atoi(strchr(s, ';') + 1) - 1;
	g_cursor.y = atoi(strchr(s, '[') + 1) - 1;
	tcflush(STDIN_FILENO, TCIFLUSH);
}

/********************************/

/*********** screen *************/

int		update_screen_size(void)
{
	struct winsize w;

	if (ioctl(STDIN_FILENO, TIOCGWINSZ, &w) == -1)
		return (1);
	g_ttysize.x = w.ws_col;
	g_ttysize.y = w.ws_row;
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

void	refresh_buffer(int rows, int cols, char *buffer, size_t count)
{
	size_t	z;

	set_cursor(rows, cols);
	z = 0;
	while (z < count)
	{
		write(STDOUT_FILENO, buffer + z, 1);
		cols += 1;
		if (cols >= g_ttysize.x)
		{
			cols = 0;
			rows += 1;
		}
		if (rows >= g_ttysize.y)
		{
			g_pactual.y -= 1;
			rows -= 1;
			tputs(tgetstr("sf", 0), 1, putchar_);
		}
		z += 1;
	}
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

/********************************/

/********keyboard event**********/

void	default_event(uint64_t c, int r)
{

}

void	ctrl_event(uint64_t c, int r)
{

}

void	meta_event(uint64_t c, int r)
{

}

void	special_event(uint64_t c, int r)
{

}

void	keyboard_event(uint64_t	c, int r)
{
	static void	(*f[])(uint64_t, int) = {
		default_event, ctrl_event, meta_event, special_event};
	int			event;

	event = IS_BACKSPACE_KEYCODE(c) * 4;
	event += IS_SPECIAL_KEYCODE(c) * 3;
	event += IS_META_MODIFIER(c) * 2;
	event += IS_CTRL_MODIFIER(c);
//	f[event](c, r);
}

/********************************/

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
//	signal(SIGINT, );
	while (42)
	{
//		signal(SIGWINCH, );
		r = read(STDIN_FILENO, &c, sizeof(c));
		keyboard_event(c, r);
		dprintf(1, "%lx\n", c);
		if (c == 'q')
			break ;
		c = 0;
	}
	revert_manual_ttymode();
	return (g_buffer.s);
}

int		main(int ac, char **av, char **envp)
{
	prompt_user();
}
