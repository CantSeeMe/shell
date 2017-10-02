/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   gprompt.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: root <marvin@42.fr>                        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2017/07/10 15:04:51 by root              #+#    #+#             */
/*   Updated: 2017/09/20 19:19:36 by jye              ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft_readline.h"
#include "lst.h"
#include "hashlib.h"

#include <termios.h>
#include <stddef.h>
#include <stdlib.h>

struct termios	g_otermios;
char			*g_prompt;
t_buff			g_buffer;

t_curs			g_winsize;

size_t			g_psize;

t_lst			*g_record;
t_lst			*g_chronicle;
t_lst			*g_chroncur;
char			*g_yank;
size_t			g_yanksize;
int				g_trigger;
void			(*g_last_action)();
