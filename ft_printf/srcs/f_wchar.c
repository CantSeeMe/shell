/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   f_wchar.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jye <marvin@42.fr>                         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2016/12/04 18:45:59 by jye               #+#    #+#             */
/*   Updated: 2017/04/22 21:21:08 by jye              ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft_printf.h"
#include <unistd.h>

static void	pp_handler__(t_format *c_flag, t_conv *tmp)
{
	int		pad;

	pad = c_flag->pad - tmp->size;
	if (c_flag->flag & 2)
	{
		write_buf(tmp->content, tmp->size);
		if (pad > 0)
			print_pp(pad, tmp->cpad);
	}
	else
	{
		if (pad > 0)
			print_pp(pad, tmp->cpad);
		write_buf(tmp->content, tmp->size);
	}
}

void		f_wchar(t_format *c_flag, va_list arg)
{
	int		wchar;
	char	a[5];
	t_conv	tmp;

	if ((c_flag->flag & 10) == 10)
		c_flag->flag ^= 8;
	tmp.cpad = c_flag->flag & 8 ? 0x30 : 0x20;
	wchar = va_arg(arg, int);
	tmp.size = w_char(wchar, a);
	tmp.content = a;
	if (c_flag->pad != 0)
		pp_handler__(c_flag, &tmp);
	else
		write_buf(a, tmp.size);
}
