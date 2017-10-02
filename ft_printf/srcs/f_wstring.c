/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   f_wchar.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jye <marvin@42.fr>                         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2016/12/04 18:45:59 by jye               #+#    #+#             */
/*   Updated: 2017/04/22 21:20:47 by jye              ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft_printf.h"
#include <unistd.h>
#include <stdlib.h>

static unsigned long int	ft_wstrlen(const int *str)
{
	register const int *cp;

	if (str == NULL)
		return (0);
	cp = str;
	while (*cp)
		++cp;
	return (cp - str);
}

static unsigned long int	wchar_conv(t_format *c_flag,
										char *bstr, const int *qstr)
{
	unsigned long int	offset;
	int					prec;
	int					word;

	if (qstr == NULL)
		return (6);
	offset = 0;
	if (c_flag->flag & 32)
	{
		prec = c_flag->precision;
		while (*qstr)
		{
			word = w_char(*qstr++, bstr + offset);
			offset += word;
			if (offset > (unsigned int)prec)
				return (offset - word);
		}
	}
	else
		while (*qstr)
			offset += w_char(*qstr++, bstr + offset);
	return (offset);
}

static void					pp_handler(t_format *c_flag, t_conv *tmp)
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

void						f_wstring(t_format *c_flag, va_list arg)
{
	int					*wchar;
	char				*a;
	t_conv				tmp;
	unsigned long int	w_len;

	wchar = va_arg(arg, int *);
	a = NULL;
	if (wchar == NULL)
		a = SNULL;
	w_len = ft_wstrlen(wchar);
	if (a == NULL)
		if ((a = ft_strnew(w_len * 4)) == NULL)
			exit(EXIT_FAILURE);
	if ((c_flag->flag & 10) == 10)
		c_flag->flag ^= 8;
	tmp.cpad = c_flag->flag & 8 ? 0x30 : 0x20;
	tmp.size = wchar_conv(c_flag, a, wchar);
	tmp.content = a;
	if (c_flag->pad != 0)
		pp_handler(c_flag, &tmp);
	else
		write_buf(a, tmp.size);
	if (wchar != NULL)
		free(a);
}
