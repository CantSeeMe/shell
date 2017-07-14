/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   prompt.h                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: root <marvin@42.fr>                        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2017/07/10 15:04:54 by root              #+#    #+#             */
/*   Updated: 2017/07/14 09:42:46 by root             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef PROMPT_H
# define PROMPT_H

# define DEFAULT_BUFFER_SIZE 4096

# define IS_SPECIAL_KEYCODE(x) (((x) & 0x5b1b) == 0x5b1b)
# define IS_META_MODIFIER(x) ((x) == 0x1b)
# define IS_BACKSPACE_KEYCODE(x) ((x) == 0x7f)
# define IS_CTRL_MODIFIER(x) ((x) < 0x1b)

# define KEYCODE_ARROW_UP		0x415b1b
# define KEYCODE_ARROW_DOWN		0x425b1b
# define KEYCODE_ARROW_RIGHT	0x435b1b
# define KEYCODE_ARROW_LEFT		0x445b1b
# define KEYCODE_CTRL_UP		0x41353b315b1b
# define KEYCODE_CTRL_DOWN		0x42353b315b1b
# define KEYCODE_CTRL_RIGHT		0x43353b315b1b
# define KEYCODE_CTRL_LEFT		0x44353b315b1b
# define KEYCODE_PAGE_UP		0x7e355b1b
# define KEYCODE_PAGE_DOWN		0x7e365b1b
# define KEYCODE_ORIGIN			0x485b1b
# define KEYCODE_END			0x465b1b
# define KEYCODE_DELETE			0x7e335b1b
# define KEYCODE_BACKSPACE		0x7f

# define KEYCODE_CTRL_Z			0x1a
# define KEYCODE_CTRL_C			0x3
# define KEYCODE_CTRL_L			0xc
# define KEYCODE_CTRL_K			0xb
# define KEYCODE_CTRL_A			0x1
# define KEYCODE_CTRL_E			0x5
# define KEYCODE_CTRL_F			0x6
# define KEYCODE_CTRL_B			0x2
# define KEYCODE_CTRL_Y			0x19
# define KEYCODE_CTRL_W			0x17
# define KEYCODE_CTRL_U			0x15

# include <stddef.h>
# include <unistd.h>

typedef struct	s_xy
{
	int		x;
	int		y;
}				t_xy;

typedef struct	s_bufhist
{
	char	*buf;
	ssize_t	bufsize;
	int		start;
}				t_bufhist;

typedef struct	s_buffer
{
	char	*s;
	char	*head;
	size_t	msize;
	size_t	len;
}				t_buffer;

# include "lst.h"
# include "hashlib.h"

# include <termios.h>

extern struct termios	g_otermios;
extern t_buffer			g_buffer;
extern t_xy				g_ttysize;
extern t_xy				g_cursor;
extern t_xy				g_pactual;
extern size_t			g_psize;
extern t_lst			*g_bufhist;
extern t_lst			*g_o_phist;
extern t_lst			*g_c_phist;
extern char				*g_yank;
extern int				g_cubuf;
extern int				g_bufhist_trigger;

#endif
