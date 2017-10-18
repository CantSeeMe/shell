/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   buff_motion10.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: root <marvin@42.fr>                        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2017/10/18 03:05:43 by root              #+#    #+#             */
/*   Updated: 2017/10/18 03:12:01 by root             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft_readline.h"

void	buff_down(void)
{
	int		cu;

	cu = g_buffer.cu;
	cu += g_winsize.col;
	if (cu > (int)g_buffer.len)
		cu = g_buffer.len;
	shift_cursor(g_buffer.cu, cu);
	g_buffer.cu = cu;
}

void	buff_up(void)
{
	int		cu;

	cu = g_buffer.cu;
	cu -= g_winsize.col;
	if (cu < 0)
		cu = 0;
	shift_cursor(g_buffer.cu, cu);
	g_buffer.cu = cu;
}
