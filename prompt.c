/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   prompt.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: root <marvin@42.fr>                        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2017/07/10 15:04:56 by root              #+#    #+#             */
/*   Updated: 2017/07/12 15:49:22 by root             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <termios.h>
#include <cusrses.h>
#include <term.h>
#include <unistd.h>
#include <stdlib.h>
#include <stddef.h>
#include <stdint.h>

#include "prompt.h"
#include "lst.h"

int		putchar_(char c)
{
	return (write(STDERR_FILENO, &c, 1));
}

/******** termios part **********/

int		set_manual_ttymode(void)
{
	struct termios termios;

	tcgetattr(STDIN_FILENO, &termios);
	g_otermios = termios;
	termios.lflag &= ~(ISIG | ECHO | ECHOE | ICANON);
	return (tcsetattr(STDIN_FILENO, &termios, TCSADRAIN));

}

int		set_original_ttymode(void)
{
	return (tcsetattr(STDIN_FILENO, &g_termios, TCSADRAIN))
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
}

/********************************/

/*********** screen *************/

int		update_screen_size(void)
{
	struct winsize w;

	if (ioctl(STDIN_FILENO, TIOGCWINSZ, &w) == -1)
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


void	keyboard_event()
{
	static ..[];
	int		event;

	event = ()
}

char	*prompt_user()
{
	uint64_t	c;
	ssize_t		r;

//	signal(SIGINT, );
	if (set_manual_ttymode())
	{
		// tcsetattr failed, setting back original termios attribute
		tcsetattr(STDIN_FILENO, &g_otermios, TCSANOW);
		return (0);
	}
	
	
	while (42)
	{
//		signal(SIGWINCH, );
		r = read(STDIN_FILENO, &c, sizeof(c));
//		some action here
		c = 0;
	}
	return (g_buffer.s);
}
