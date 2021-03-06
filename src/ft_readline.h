/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   prompt.h                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: root <marvin@42.fr>                        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2017/07/10 15:04:54 by root              #+#    #+#             */
/*   Updated: 2017/10/23 09:28:42 by jye              ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef FT_READLINE_H
# define FT_READLINE_H

# define DEFAULT_BUFFER_SIZE 4096

# define RL_ACTION_MASK			0xff
# define RL_ACTION_WRITE		0x00
# define RL_ACTION_DELETE		0x01
# define RL_ACTION_OVERWRITE	0x02
# define RL_ACTION_MOVE			0x03

# define RL_CURSOR_MASK			0xff00
# define RL_CURSOR_RESET		0x0000
# define RL_CURSOR_KEEP			0x0100

# define IS_SPECIAL_KEYCODE(x)		(((x) & 0x5b1b) == 0x5b1b)
# define IS_META_MODIFIER(x)		((x) == 0x1b)
# define IS_BACKSPACE_KEYCODE(x)	((x) == 0x7f)
# define IS_CTRL_MODIFIER(x)		((x) < 0x20)

# define WORD_UCHAR	"ABCDEFGHIJKLMNOPQRSTUVWXYZ"
# define WORD_LCHAR	"abcdefghijklmnopqrstuvwxyz"
# define WORD_SCHAR	"._-"
# define WORD_NCHAR	"1234567890"

# define WORD_ANCHAR	WORD_UCHAR WORD_LCHAR WORD_NCHAR
# define WORD_FCHAR		WORD_ANCHAR WORD_SCHAR "/"

# define KEYCODE_ARROW_UP		0x415b1b
# define KEYCODE_ARROW_DOWN		0x425b1b
# define KEYCODE_ARROW_RIGHT	0x435b1b
# define KEYCODE_ARROW_LEFT		0x445b1b

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
**# define KEYCODE_CTRL_Z			0x1a ignored
**# define KEYCODE_CTRL_C			0x3 with kill
**# define KEYCODE_CTRL_D			0x4 done
**# define KEYCODE_CTRL_L			0xc done
**# define KEYCODE_CTRL_K			0xb done
**# define KEYCODE_CTRL_A			0x1 done
**# define KEYCODE_CTRL_B			0x2 done
**# define KEYCODE_CTRL_E			0x5 done
**# define KEYCODE_CTRL_F			0x6 done
**# define KEYCODE_CTRL_Y			0x19 done
**# define KEYCODE_CTRL_W			0x17 done
**# define KEYCODE_CTRL_U			0x15 done
**# define KEYCODE_CTRL_SLASH		0x1f done
**# define KEYCODE_ENTER			0xa done
*/

# include "hashlib.h"
# include "lst.h"
# include "typedef_ft_readline.h"

# include <termios.h>
# include <stddef.h>
# include <unistd.h>

char					*ft_readline(char *prompt, size_t psize);
char					*expansion_readline(char *s, char *pr, size_t prsize);

extern struct termios	g_otermios;
extern char				*g_prompt;
extern t_buff			g_buffer;

extern t_curs			g_winsize;

extern size_t			g_psize;

extern t_lst			*g_record;
extern t_lst			*g_chronicle;
extern t_lst			*g_chroncur;
extern char				*g_yank;
extern size_t			g_yanksize;
extern void				(*g_last_action)();

/*
** buffer motion
*/

void					buff_head(void);
void					buff_end(void);
void					buff_prev(void);
void					buff_next(void);
void					buff_down(void);
void					buff_up(void);
void					buff_prev_word(void);
void					buff_next_word(void);

void					buff_autocomplete(void);
void					buff_capitalize(void);
void					buff_lowtoup(void);
void					buff_uptolow(void);

void					buff_del_word(void);
void					buff_del_next(void);
void					buff_del_prev(void);
void					buff_kill_next(void);
void					buff_kill_prev(void);

void					buff_refresh(int cursor, char *buff, ssize_t slen);
int						buff_malloc(size_t mlen);
int						buff_realloc(size_t mlen);
void					buff_insert(void *c, int r);
void					buff_newline(void);
void					buff_delete(void);
void					buff_reset_state(void);
void					buff_clear_content(void);

void					buff_record(int start, ssize_t bufsize, int action);
void					buff_revert(void);

void					buff_yankin(void);
void					buff_yankout(void);

void					buff_chronicup(void);
void					buff_chronicdown(void);

/*
** screen cursor motion
*/

void					shift_cursor(int current, int target);

/*
**etc
*/
void					keyboard_event(uint64_t c, int r);
void					exit_readline(void);
void					reset_readline(void);
void					reload_line(int sig);
int						update_winsize(void);

int						putchar_(int c);

#endif
