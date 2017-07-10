/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   prompt.h                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: root <marvin@42.fr>                        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2017/07/10 15:04:54 by root              #+#    #+#             */
/*   Updated: 2017/07/10 15:58:58 by root             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef PROMPT_H
# define PROMPT_H

typedef struct	s_xy
{
	int		x;
	int		y;
}				t_xy;

typedef struct	s_bufhist
{
	char	*buf;
	int		original;
	int		current;
}				t_bufhist;

# include "lst.h"
# include "hashlib.h"

# include <termios.h>

extern struct termios	g_otermios;
extern t_xy				g_ttysize;
extern t_xy				g_cursor;
extern t_xy				g_pactual;
extern t_xy				g_pabsolute;
extern size_t			g_bufsize;
extern size_t			g_promptsize;
extern t_lst			*g_bufhist;
extern t_lst			*g_prompthist;

#endif
