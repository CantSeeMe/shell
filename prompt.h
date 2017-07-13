/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   prompt.h                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: root <marvin@42.fr>                        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2017/07/10 15:04:54 by root              #+#    #+#             */
/*   Updated: 2017/07/12 11:10:06 by root             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef PROMPT_H
# define PROMPT_H

# define DEFAULT_BUFFER_SIZE 4096

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
