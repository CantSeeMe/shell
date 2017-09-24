/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_readline1.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jye <jye@student.42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2017/09/20 19:30:02 by jye               #+#    #+#             */
/*   Updated: 2017/09/24 02:07:58 by jye              ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft_readline.h"

#include <stddef.h>
#include <stdint.h>

static void	place_holder(void)
{
	return ;
}

static void	ctrl_event(uint64_t c)
{
	static void (*f[])() = {
		buff_head, buff_prev, reset_readline, buff_delete, buff_end,
		buff_next, place_holder, place_holder,
		buff_autocomplete, exit_readline,
		buff_kill_next, buff_reset_state,
		place_holder, place_holder, place_holder,
		place_holder, place_holder, place_holder, place_holder, place_holder,
		buff_kill_prev, place_holder, buff_del_word, place_holder, buff_yankout,
		place_holder, place_holder, place_holder, place_holder, place_holder,
		buff_revert
	};

	f[c - 1]();
	g_last_action = f[c - 1];
}

/*
**f forward word
**b backward word
**r reset history and prompt
**u upper case
**l lower case
**c forward word upper first letter
*/

static void	meta_event(void)
{
	uint64_t	c;

	c = 0;
	read(STDIN_FILENO, &c, sizeof(c));
	if (c == 'f')
		(g_last_action = buff_next_word)();
	else if (c == 'b')
		(g_last_action = buff_prev_word)();
	else if (c == 'u')
		(g_last_action = buff_lowtoup)();
	else if (c == 'l')
		(g_last_action = buff_uptolow)();
	else if (c == 'r')
		(g_last_action = buff_reset_state)();
	else if (c == 'c')
		(g_last_action = buff_capitalize)();
}

static void	special_event(uint64_t c)
{
	static void (*special1[8])() = {
		buff_chronicup, buff_chronicdown, buff_next, buff_prev,
		place_holder, buff_end, place_holder, buff_head};
	static void (*special2[4])() = {
		place_holder, place_holder, buff_next_word, buff_prev_word};
	static void (*special3[4])() = {
		buff_del_next, place_holder, place_holder, place_holder};

	c = c >> 16;
	if (c >= 0x41 && c <= 0x48)
	{
		(g_last_action = special1[c - 0x41])();
	}
	else if (IS_CTRL_ARROW(c))
	{
		(g_last_action = special2[(c >> 24) - 0x41])();
	}
	else if ((c & 0x7e30) == 0x7e30 &&
			((c & 0xf) > 2 && (c & 0xf) <= 6))
	{
		(g_last_action = special3[(c ^ 0x7e30) - 0x3])();
	}
}

void		keyboard_event(uint64_t c, int r)
{
	if (IS_BACKSPACE_KEYCODE(c))
		buff_del_prev();
	else if (IS_SPECIAL_KEYCODE(c))
		special_event(c);
	else if (IS_META_MODIFIER(c))
		meta_event();
	else if (IS_CTRL_MODIFIER(c))
		ctrl_event(c);
	else
		buff_insert(&c, r);
	buff_yankin();
}
