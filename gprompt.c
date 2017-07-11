/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   gprompt.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: root <marvin@42.fr>                        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2017/07/10 15:04:51 by root              #+#    #+#             */
/*   Updated: 2017/07/11 17:29:58 by root             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "prompt.h"
#include "lst.h"
#include "hashlib.h"

#include <termios.h>

struct termios	g_otermios;
t_xy			g_ttysize;
t_xy			g_cursor;
t_xy			g_pactual;
size_t			g_bufsize;
size_t			g_promptsize;
t_lst			*g_bufhist;
t_lst			*g_o_phist;
t_lst			*g_c_phist;
int				g_cubuf;
int				g_bufhist_trigger;
