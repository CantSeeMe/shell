/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   prompt.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: root <marvin@42.fr>                        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2017/07/10 15:04:56 by root              #+#    #+#             */
/*   Updated: 2017/09/20 19:39:50 by jye              ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <termios.h>
#include <curses.h>
#include <term.h>
#include <unistd.h>
#include <stddef.h>
#include <stdint.h>
#include <string.h>
#include <signal.h>

#include "ft_readline.h"
#include "lst.h"
#include "htvar.h"

static int	set_manual_ttymode(void)
{
	struct termios termios;

	tcgetattr(STDIN_FILENO, &termios);
	g_otermios = termios;
	termios.c_oflag &= ~OPOST;
	termios.c_lflag &= ~(ECHO | ECHONL | ICANON | ISIG | IEXTEN);
	return (tcsetattr(STDIN_FILENO, TCSANOW, &termios));
}

static int	revert_manual_ttymode(void)
{
	return (tcsetattr(STDIN_FILENO, TCSANOW, &g_otermios));
}

static int	init_readline(void)
{
	t_chronicle	*chro;

	g_buffer.cu = 0;
	g_buffer.len = 0;
	if ((update_winsize()))
		return (1);
	if ((chro = malloc(sizeof(*chro))) == 0)
		return (1);
	memset(chro, 0, sizeof(*chro));
	if (g_chronicle && ((t_chronicle *)g_chronicle->data)->s == 0)
		free(chro);
	else if (push_lst__(&g_chronicle, chro))
	{
		free(chro);
		return (1);
	}
	g_chroncur = g_chronicle;
	return (buff_malloc(DEFAULT_BUFFER_SIZE));
}

static void	update_prompt(char *prompt, size_t psize)
{
	g_prompt = prompt;
	g_psize = psize;
	write(2, prompt, strlen(prompt));
}

char		*ft_readline(char *prompt, size_t psize)
{
	uint64_t	c;
	int			r;

	if (init_readline())
		return ((char *)0);
	if (set_manual_ttymode())
	{
		free(g_buffer.s);
		tcsetattr(STDIN_FILENO, TCSADRAIN, &g_otermios);
		return ((char *)0);
	}
	tgetent(vhash_search("TERM"), NULL);
	update_prompt(prompt, psize);
	while (g_buffer.msize)
	{
		signal(SIGWINCH, reload_line);
		c = 0;
		r = read(STDIN_FILENO, &c, sizeof(c));
		keyboard_event(c, r);
	}
	revert_manual_ttymode();
	return (g_buffer.s);
}
