/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   prompt.h                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: root <marvin@42.fr>                        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2017/07/10 15:04:54 by root              #+#    #+#             */
/*   Updated: 2017/07/26 17:55:03 by root             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef PROMPT_H
# define PROMPT_H

# define DEFAULT_BUFFER_SIZE 4096

# define IS_SPECIAL_KEYCODE(x) (((x) & 0x5b1b) == 0x5b1b)
# define IS_META_MODIFIER(x) ((x) == 0x1b)
# define IS_BACKSPACE_KEYCODE(x) ((x) == 0x7f)
# define IS_CTRL_MODIFIER(x) ((x) < 0x20)
/*
**# define KEYCODE_ARROW_UP		0x41
**# define KEYCODE_ARROW_DOWN		0x42
**# define KEYCODE_ARROW_RIGHT	0x43
**# define KEYCODE_ARROW_LEFT		0x44
*/
# define IS_CTRL_ARROW(x) (((x) & 0x353b31) == 0x353b31)
/*
**# define KEYCODE_CTRL_UP		0x41
**# define KEYCODE_CTRL_DOWN		0x42
**# define KEYCODE_CTRL_RIGHT		0x43
**# define KEYCODE_CTRL_LEFT		0x44
*/
/*
**# define KEYCODE_PAGE_UP		0x7e35
**# define KEYCODE_PAGE_DOWN		0x7e36
**# define KEYCODE_ORIGIN			0x48
**# define KEYCODE_END			0x46
**# define KEYCODE_DELETE			0x7e33
*/
/*
**# define KEYCODE_BACKSPACE		0x7f
*/
/*
**# define KEYCODE_CTRL_Z			0x1a
**# define KEYCODE_CTRL_C			0x3
**# define KEYCODE_CTRL_L			0xc
**# define KEYCODE_CTRL_K			0xb
**# define KEYCODE_CTRL_A			0x1
**# define KEYCODE_CTRL_B			0x2
**# define KEYCODE_CTRL_E			0x5
**# define KEYCODE_CTRL_F			0x6
**# define KEYCODE_CTRL_Y			0x19
**# define KEYCODE_CTRL_W			0x17
**# define KEYCODE_CTRL_U			0x15
**# define KEYCODE_CTRL_SLASH		0x1f
*/

# include <stddef.h>
# include <unistd.h>

typedef struct	s_curs
{
	int		row;
	int		col;
}				t_curs;

typedef struct	s_record
{
	char	*buf;
	ssize_t	bufsize;
	int		start;
}				t_record;

/* typedef struct	s_chronicle */
/* { */
/* 	t_lst		*record; */
/* }				t_curs; */

typedef struct	s_buff
{
	char	*s;
	char	*head;
	size_t	msize;
	size_t	len;
}				t_buff;

# include "lst.h"
# include "hashlib.h"

# include <termios.h>

extern struct termios	g_otermios;
extern char				*g_prompt;
extern t_buff			g_buffer;

extern t_curs			g_curs;
extern t_curs			g_winsize;

extern size_t			g_psize;
extern size_t			g_cubuf;

extern t_lst			*g_record;
extern t_lst			*g_chronicle;
extern char				*g_yank;
extern int				g_trigger;

/*
** buffer motion
*/

void	buff_head(void);
void	buff_end(void);
void	buff_prev(void);
void	buff_next(void);
void	buff_prev_word(void);
void	buff_next_word(void);

/*
** screen cursor motion
*/

void	shift_cursor(int s);

#endif
