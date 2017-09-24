/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   f_undefined.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jye <marvin@42.fr>                         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2016/12/11 20:32:26 by jye               #+#    #+#             */
/*   Updated: 2017/04/22 21:25:22 by jye              ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft_printf.h"

static void	pp_handler(t_format *c_flag, t_conv *tmp)
{
	int		pad;

	pad = c_flag->pad - tmp->size;
	if (c_flag->flag & 2)
	{
		write_buf(tmp->content, 1);
		if (pad > 0)
			print_pp(pad, tmp->cpad);
	}
	else
	{
		if (pad > 0)
			print_pp(pad, tmp->cpad);
		write_buf(tmp->content, 1);
	}
}

void		f_undefined(t_format *c_flag)
{
	t_conv	tmp;
	char	undef;

	if ((c_flag->flag & 10) == 10)
		c_flag->flag ^= 8;
	tmp.cpad = c_flag->flag & 8 ? 0x30 : 0x20;
	tmp.size = 1;
	undef = c_flag->format;
	tmp.content = &undef;
	if (c_flag->pad != 0)
	{
		pp_handler(c_flag, &tmp);
		return ;
	}
	if (undef)
		write_buf(&undef, 1);
}
