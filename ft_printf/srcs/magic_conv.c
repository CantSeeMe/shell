/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   magic_conv.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jye <marvin@42.fr>                         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2016/12/03 00:00:00 by jye               #+#    #+#             */
/*   Updated: 2017/04/22 21:12:28 by jye              ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft_printf.h"

static void	some_other_shit(t_format *c_flag, va_list arg)
{
	(void)arg;
	if (c_flag->format == 'b')
		f_ubint(c_flag, arg);
	else
		f_undefined(c_flag);
}

static void	n_conv(t_format *c_flag, va_list arg)
{
	void *conv;

	conv = va_arg(arg, void *);
	if (c_flag->length & 60)
		*((unsigned long long *)conv) = get_len();
	else if (c_flag->length & 2)
		*((unsigned short *)conv) = get_len();
	else if (c_flag->length & 1)
		*((unsigned char *)conv) = get_len();
	else
		*((unsigned int *)conv) = get_len();
}

void		magic_conv(t_format *c_flag, va_list arg)
{
	(void)arg;
	if (c_flag->format == 'c')
		f_char(c_flag, arg);
	else if (c_flag->format == 's')
		f_string(c_flag, arg);
	else if (c_flag->format == 'C')
		f_wchar(c_flag, arg);
	else if (c_flag->format == 'S')
		f_wstring(c_flag, arg);
	else if (c_flag->format == 'p')
		f_pint(c_flag, arg);
	else if (c_flag->format == 'n')
		n_conv(c_flag, arg);
	else if (c_flag->format == 'D' || c_flag->format == 'd' ||
			c_flag->format == 'i')
		f_sint(c_flag, arg);
	else if (c_flag->format == 'U' || c_flag->format == 'u')
		f_uint(c_flag, arg);
	else if (c_flag->format == 'O' || c_flag->format == 'o')
		f_uoint(c_flag, arg);
	else if (c_flag->format == 'X' || c_flag->format == 'x')
		f_uxint(c_flag, arg);
	else
		some_other_shit(c_flag, arg);
}
