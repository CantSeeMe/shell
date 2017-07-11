/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   prompt.h                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: root <marvin@42.fr>                        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2017/07/10 15:04:54 by root              #+#    #+#             */
/*   Updated: 2017/07/11 18:42:17 by root             ###   ########.fr       */
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

typedef struct	s_prompt
{
	char	**s;
	char	**c;
	
}				t_prompt;

# include "lst.h"
# include "hashlib.h"

# include <termios.h>

extern struct termios	g_otermios;
extern t_xy				g_ttysize;
extern t_xy				g_cursor;
extern t_xy				g_pactual;
extern size_t			g_bufsize;
extern size_t			g_promptsize;
extern t_lst			*g_bufhist;
extern t_lst			*g_o_phist;
extern t_lst			*g_c_phist;
extern int				g_cubuf;
extern int				g_bufhist_trigger;

#endif
