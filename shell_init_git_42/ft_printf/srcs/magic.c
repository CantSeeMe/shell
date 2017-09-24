/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   magic.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jye <marvin@42.fr>                         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2016/12/06 15:28:06 by jye               #+#    #+#             */
/*   Updated: 2016/12/13 19:56:13 by jye              ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft_printf.h"

static int	s_flag(char **format)
{
	register char	*f;
	register char	*flag;
	register int	z;

	f = *format;
	flag = FLAG;
	z = 0;
	while (*f == flag[0] || *f == flag[1] || *f == flag[2] || *f == flag[3]
			|| *f == flag[4])
	{
		if (*f == flag[0])
			z |= 1;
		else if (*f == flag[1])
			z |= 2;
		else if (*f == flag[2])
			z |= 4;
		else if (*f == flag[3])
			z |= 8;
		else if (*f == flag[4])
			z |= 16;
		++f;
	}
	*format = f;
	return (z);
}

static int	s_gint(char **format, t_format *c_flag, va_list arg,
					char bool_prec__)
{
	register char	*f;
	register int	nb;

	f = *format;
	nb = 0;
	if (**format == 0x2a)
	{
		*format += 1;
		if (((nb = va_arg(arg, int)) & 0x80000000) && !bool_prec__)
		{
			nb = (0x80000000 - (nb ^ 0x80000000));
			c_flag->flag |= 2;
		}
		return (nb);
	}
	while (*f && (*f >= 0x30 && *f <= 0x39))
	{
		nb = nb * 10 + (*f - 0x30);
		++f;
	}
	*format = f;
	return (nb);
}

static int	s_glen(char **format)
{
	unsigned short	*z;
	char			*f;
	char			*lenc;
	int				len;

	z = (unsigned short *)*format;
	f = *format;
	lenc = LENGTH;
	len = 0;
	while ((*f == lenc[0] || *f == lenc[1] || *f == lenc[2] || *f == lenc[3])
			&& (z = (unsigned short *)f))
		if (*z == BYTE && (f += 2))
			len |= 1;
		else if (*z == QWORD && (f += 2))
			len |= 8;
		else if (*f == WORD && (f += 1))
			len |= 2;
		else if (*f == DWORD && (f += 1))
			len |= 4;
		else if (*f == SIZE && (f += 1))
			len |= 16;
		else if (*f == MAX && (f += 1))
			len |= 32;
	*format = f;
	return (len);
}

void		magic(t_format *c_flag, char **format, va_list arg)
{
	int tmp;

	(void)arg;
	c_flag->flag = 0;
	c_flag->pad = 0;
	c_flag->length = 0;
	c_flag->format = 0;
	c_flag->precision = 0;
	while (1)
	{
		c_flag->flag |= s_flag(format);
		tmp = s_gint(format, c_flag, arg, 0);
		c_flag->pad = tmp != 0 ? tmp : c_flag->pad;
		if (**format == 0x2e && (c_flag->flag |= 32))
		{
			(*format) += 1;
			c_flag->precision = s_gint(format, c_flag, arg, 1);
		}
		c_flag->length |= s_glen(format);
		if (**format && ft_strchr(RESET, **format))
			continue ;
		c_flag->format = **format;
		*format += **format ? 1 : 0;
		return ;
	}
}
