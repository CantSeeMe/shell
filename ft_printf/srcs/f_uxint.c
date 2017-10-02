/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   f_uxint.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jye <marvin@42.fr>                         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2016/12/08 18:26:46 by jye               #+#    #+#             */
/*   Updated: 2017/04/22 21:25:12 by jye              ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft_printf.h"

static unsigned long long	word_mlen(t_format *c_flag, va_list arg)
{
	unsigned long long conv;

	if (c_flag->length & 60)
		conv = va_arg(arg, long long);
	else if (c_flag->length & 2)
		conv = 0xffff & va_arg(arg, int);
	else if (c_flag->length & 1)
		conv = 0xff & va_arg(arg, int);
	else
		conv = 0xffffffff & va_arg(arg, int);
	return (conv);
}

static void					f_handler__(t_format *c_flag, t_conv *tmp,
										char bool__)
{
	char	*buff;
	int		i;

	if (c_flag->flag & 1 && tmp->size && ((char *)tmp->content)[0] != 0x30)
	{
		if (c_flag->format == 'x')
			write_buf("0x", 2);
		else
			write_buf("0X", 2);
	}
	if (c_flag->format == 'X')
	{
		i = tmp->size;
		buff = tmp->content;
		while (i-- > 0)
		{
			if (*buff >= 0x61 && *buff <= 0x66)
				*buff = *buff - 0x20;
			++buff;
		}
	}
	if (!bool__)
		write_buf(tmp->content, tmp->size);
}

static void					handler__(t_format *c_flag, t_conv *tmp,
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
	if (c_flag->flag & 1)
		lpad -= 2;
	handler__(c_flag, tmp, lpad, lprec);
}

void						f_uxint(t_format *c_flag, va_list arg)
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
	conv = word_mlen(c_flag, arg);
	tmp.size = f_utox(c_flag, conv, buff);
	tmp.content = buff;
	if (c_flag->pad || c_flag->precision)
		pp_handler__(c_flag, &tmp);
	else if (c_flag->flag & 1 || c_flag->format == 'X')
		f_handler__(c_flag, &tmp, 0);
	else
		write_buf(buff, tmp.size);
}
