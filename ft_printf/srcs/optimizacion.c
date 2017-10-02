/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   optimizacion.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jye <marvin@42.fr>                         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2016/12/12 20:50:38 by jye               #+#    #+#             */
/*   Updated: 2017/09/30 21:40:00 by jye              ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft_printf.h"
#include <stdlib.h>
#include <unistd.h>
#include <stdio.h>
#define IO_SIZE 4096

static char		g_buff[IO_SIZE];
static int		g_fd;
static int		g_i;
static size_t	g_len;

void		print_pp(int lpp, char cpad)
{
	if (lpp + g_i > IO_SIZE)
		print_buf();
	if (lpp > IO_SIZE)
	{
		ft_memset(g_buff, cpad, IO_SIZE);
		while (lpp > IO_SIZE)
		{
			write(g_fd, g_buff, IO_SIZE);
			g_len += IO_SIZE;
			lpp -= IO_SIZE;
		}
		write(g_fd, g_buff, lpp);
		g_len += lpp;
	}
	else
	{
		ft_memset(g_buff + g_i, cpad, lpp);
		g_i += lpp;
		g_len += lpp;
	}
}

void		write_buf(void *s, unsigned long n)
{
	if (n + g_i > IO_SIZE)
		print_buf();
	if (n > IO_SIZE)
	{
		while (n > IO_SIZE)
		{
			ft_memcpy(g_buff, s, IO_SIZE);
			write(g_fd, g_buff, IO_SIZE);
			s += IO_SIZE;
			n -= IO_SIZE;
			g_len += IO_SIZE;
		}
		g_i = n;
		ft_memcpy(g_buff, s, n);
		g_len += n;
	}
	else
	{
		ft_memcpy(g_buff + g_i, s, n);
		g_i += n;
		g_len += n;
	}
}

void		print_buf(void)
{
	write(g_fd, g_buff, g_i);
	g_i = 0;
}

size_t		get_len(void)
{
	return (g_len);
}

void		init_buf(int fd)
{
	g_i = 0;
	g_fd = fd;
	g_len = 0;
}
