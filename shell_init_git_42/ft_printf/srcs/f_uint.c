/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   f_uint.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jye <marvin@42.fr>                         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2016/12/08 18:26:46 by jye               #+#    #+#             */
/*   Updated: 2017/04/22 21:23:56 by jye              ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft_printf.h"

static unsigned long long	word_mlen(t_format *c_flag, va_list arg)
{
	unsigned long long conv;

	if (c_flag->length & 60 || c_flag->format == 'U')
		conv = va_arg(arg, unsigned long long);
	else if (c_flag->length & 2)
		conv = 0xffff & va_arg(arg, unsigned int);
	else if (c_flag->length & 1)
		conv = 0xff & va_arg(arg, unsigned int);
	else
		conv = 0xffffffff & va_arg(arg, unsigned int);
	return (conv);
}

static void					handler__(t_format *c_flag, t_conv *tmp,
								int lpad, int lprec)
{
	if (c_flag->flag & 2)
	{
		if (lprec > 0)
			print_pp(lprec, '0');
		write_buf(tmp->content, tmp->size);
		if (lpad > 0)
			print_pp(lpad, tmp->cpad);
	}
	else
	{
		if (lpad > 0)
			print_pp(lpad, tmp->cpad);
		if (lprec > 0)
			print_pp(lprec, '0');
		write_buf(tmp->content, tmp->size);
	}
}

static void					pp_handler__(t_format *c_flag, t_conv *tmp)
{
	int		lpad;
	int		lprec;

	lpad = 0;
	lprec = 0;
	tmp->cpad = c_flag->flag & 8 ? 0x30 : 0x20;
	if (c_flag->flag & 32)
		lprec = c_flag->precision - tmp->size;
	if (lprec > 0)
		lpad = c_flag->pad - (lprec + tmp->size);
	else
		lpad = c_flag->pad - tmp->size;
	handler__(c_flag, tmp, lpad, lprec);
}

void						f_uint(t_format *c_flag, va_list arg)
{
	unsigned long long	conv;
	char				buff[21];
	t_conv				tmp;

	if (c_flag->flag & 8)
	{
		if ((c_flag->flag & 10) == 10)
			c_flag->flag ^= 8;
		else if (c_flag->flag & 32)
			c_flag->flag &= 0xf7;
	}
	conv = word_mlen(c_flag, arg);
	tmp.size = f_utoa(c_flag, conv, buff);
	tmp.content = buff;
	if (c_flag->pad || c_flag->precision)
		pp_handler__(c_flag, &tmp);
	else
		write_buf(buff, tmp.size);
}
