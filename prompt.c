/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   prompt.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: root <marvin@42.fr>                        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2017/07/10 15:04:56 by root              #+#    #+#             */
/*   Updated: 2017/07/11 18:51:57 by root             ###   ########.fr       */
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

void	refresh_cursor(void)
{

}

/********************************/

/*********** buffer *************/

void	malloc_buffer(void)
{
	
}

void	refresh_buffer(void)
{

}

void	realloc_buffer(void)
{
	
}

/********************************/

char	*prompt_user(char *prompt)
{
	uint64_t	c;
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
		read(STDIN_FILENO, &c, sizeof(c));
//		some action here
		c = 0;
	}
}
