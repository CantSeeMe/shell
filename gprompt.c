/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   gprompt.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: root <marvin@42.fr>                        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2017/07/10 15:04:51 by root              #+#    #+#             */
/*   Updated: 2017/07/13 14:22:04 by root             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "prompt.h"
#include "lst.h"
#include "hashlib.h"

#include <termios.h>
#include <stddef.h>
#include <stdlib.h>

struct termios	g_otermios;
t_buffer		g_buffer;
t_xy			g_ttysize;
t_xy			g_cursor;
t_xy			g_pactual;
size_t			g_psize;
t_lst			*g_bufhist;
t_lst			*g_o_phist;
t_lst			*g_c_phist;
char			*g_yank;
int				g_cubuf;
int				g_bufhist_trigger;
