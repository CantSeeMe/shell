/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   f_sint.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jye <marvin@42.fr>                         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2016/12/08 18:26:46 by jye               #+#    #+#             */
/*   Updated: 2017/04/22 21:23:05 by jye              ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft_printf.h"

static long long	word_mlen(t_format *c_flag, va_list arg)
{
	long long conv;

	if (c_flag->length & 60 || c_flag->format == 'D')
		conv = va_arg(arg, long long);
	else if (c_flag->length & 2)
	{
		if ((conv = 0xffff & va_arg(arg, int)) & 0x8000)
			conv = -(0x8000 - (conv ^ 0x8000));
	}
	else if (c_flag->length & 1)
	{
		if ((conv = 0xff & va_arg(arg, int)) & 0x80)
			conv = -(0x80 - (conv ^ 0x80));
	}
	else
		conv = va_arg(arg, int);
	return (conv);
}

static void			f_handler__(t_format *c_flag, t_conv *tmp, char bool__)
{
	if (tmp->sign == 1)
		write_buf("-", 1);
	else if (c_flag->flag & 4)
		write_buf("+", 1);
	else if (c_flag->flag & 16)
		write_buf(" ", 1);
	if (!bool__)
		write_buf(tmp->content, tmp->size);
}

static void			handler__(t_format *c_flag, t_conv *tmp,
								int lpad, int lprec)
{
	if (c_flag->flag & 2)
	{
		f_handler__(c_flag, tmp, 1);
		if (lprec > 0)
			print_pp(lprec, '0');
		write_buf(tmp->content, tmp->size);
		if (lpad > 0)
			print_pp(lpad, tmp->cpad);
	}
	else
	{
		if (tmp->cpad == 0x30)
			f_handler__(c_flag, tmp, 1);
		if (lpad > 0)
			print_pp(lpad, tmp->cpad);
		if (tmp->cpad != 0x30)
			f_handler__(c_flag, tmp, 1);
		if (lprec > 0)
			print_pp(lprec, '0');
		write_buf(tmp->content, tmp->size);
	}
}

static void			pp_handler__(t_format *c_flag, t_conv *tmp)
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
	if ((c_flag->flag & 20) || tmp->sign)
		lpad -= 1;
	handler__(c_flag, tmp, lpad, lprec);
}

void				f_sint(t_format *c_flag, va_list arg)
{
	long long	conv;
	char		buff[21];
	t_conv		tmp;

	if (c_flag->flag & 28)
	{
		if ((c_flag->flag & 10) == 10)
			c_flag->flag ^= 8;
		else if ((c_flag->flag & 20) == 20)
			c_flag->flag ^= 16;
		else if (c_flag->flag & 32)
			c_flag->flag &= 0xf7;
	}
	conv = word_mlen(c_flag, arg);
	tmp.sign = conv < 0L ? 1 : 0;
	tmp.size = f_itoa(c_flag, conv, buff);
	tmp.content = buff;
	if (c_flag->pad || c_flag->precision)
		pp_handler__(c_flag, &tmp);
	else if (tmp.sign || c_flag->flag & 20)
		f_handler__(c_flag, &tmp, 0);
	else
		write_buf(buff, tmp.size);
}
