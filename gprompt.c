/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   gprompt.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: root <marvin@42.fr>                        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2017/07/10 15:04:51 by root              #+#    #+#             */
/*   Updated: 2017/07/26 17:53:51 by root             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "prompt.h"
#include "lst.h"
#include "hashlib.h"

#include <termios.h>
#include <stddef.h>
#include <stdlib.h>

struct termios	 g_otermios;
char			 *g_prompt;
t_buff			 g_buffer;

t_curs			 g_curs;
t_curs			 g_winsize;

size_t			 g_psize;
size_t			 g_cubuf;

t_lst			 *g_record;
t_lst			 *g_chronicle;
char			 *g_yank;
int				 g_trigger;
