/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   gprompt.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: root <marvin@42.fr>                        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2017/07/10 15:04:51 by root              #+#    #+#             */
/*   Updated: 2017/07/10 15:59:07 by root             ###   ########.fr       */
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
t_xy			g_pabsolute;
int				g_cubuf;
size_t			g_bufsize;
size_t			g_promptsize;
t_lst			*g_bufhist;
t_lst			*g_prompthist;
