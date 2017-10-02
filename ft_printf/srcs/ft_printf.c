/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_printf.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jye <marvin@42.fr>                         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2016/11/06 00:00:00 by jye               #+#    #+#             */
/*   Updated: 2017/04/22 21:26:27 by jye              ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft_printf.h"
#include <unistd.h>

static int	magic_print(char **format)
{
	char	*f;
	char	*s_modulo;
	int		to_w;

	f = *format;
	s_modulo = ft_strchrnul(f, 0x25);
	to_w = s_modulo - f;
	write_buf(f, to_w);
	*format += to_w;
	return (to_w);
}

int			ft_printf(const char *format, ...)
{
	va_list		arg;
	t_format	c_da;

	if (format == NULL)
		return (0);
	va_start(arg, format);
	init_buf(1);
	while (*format)
	{
		if (*format == 0x25)
		{
			format += 1;
			magic(&c_da, (char **)&format, arg);
			magic_conv(&c_da, arg);
		}
		magic_print((char **)&format);
	}
	va_end(arg);
	print_buf();
	return (get_len());
}

int			ft_dprintf(const int fd, const char *format, ...)
{
	va_list		arg;
	t_format	c_da;

	if (format == NULL)
		return (0);
	va_start(arg, format);
	init_buf(fd);
	while (*format)
	{
		if (*format == 0x25)
		{
			format += 1;
			magic(&c_da, (char **)&format, arg);
			magic_conv(&c_da, arg);
		}
		magic_print((char **)&format);
	}
	va_end(arg);
	print_buf();
	return (get_len());
}
