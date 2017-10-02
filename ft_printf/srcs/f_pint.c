/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   f_pint.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jye <marvin@42.fr>                         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2016/12/08 18:26:46 by jye               #+#    #+#             */
/*   Updated: 2017/04/22 21:22:54 by jye              ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft_printf.h"

static void		handler__(t_format *c_flag, t_conv *tmp, int lpad, int lprec)
{
	if (c_flag->flag & 2)
	{
		write_buf("0x", 2);
		if (lprec > 0)
			print_pp(lprec, '0');
		write_buf(tmp->content, tmp->size);
		if (lpad > 0)
			print_pp(lpad, tmp->cpad);
	}
	else
	{
		if (tmp->cpad == 0x30)
			write_buf("0x", 2);
		if (lpad > 0)
			print_pp(lpad, tmp->cpad);
		if (tmp->cpad != 0x30)
			write_buf("0x", 2);
		if (lprec > 0)
			print_pp(lprec, '0');
		write_buf(tmp->content, tmp->size);
	}
}

static void		pp_handler__(t_format *c_flag, t_conv *tmp)
{
	int		lpad;
	int		lprec;

	lpad = -2;
	lprec = 0;
	tmp->cpad = c_flag->flag & 8 ? 0x30 : 0x20;
	if (c_flag->flag & 32)
		lprec = c_flag->precision - tmp->size;
	if (lprec > 0)
		lpad += (c_flag->pad - (lprec + tmp->size));
	else
		lpad += (c_flag->pad - tmp->size);
	if (c_flag->flag & 1)
		lpad -= 2;
	handler__(c_flag, tmp, lpad, lprec);
}

void			f_pint(t_format *c_flag, va_list arg)
{
	unsigned long long	conv;
	char				buff[17];
	t_conv				tmp;

	if (c_flag->flag & 28)
	{
		if ((c_flag->flag & 10) == 10)
			c_flag->flag ^= 8;
		else if (c_flag->flag & 32)
			c_flag->flag &= 0xf7;
	}
	conv = va_arg(arg, unsigned long long);
	tmp.size = f_utox(c_flag, conv, buff);
	tmp.content = buff;
	tmp.cpad = c_flag->flag & 8 ? 0x30 : 0x20;
	if (c_flag->pad || c_flag->precision)
	{
		pp_handler__(c_flag, &tmp);
		return ;
	}
	write_buf("0x", 2);
	write_buf(buff, tmp.size);
}
