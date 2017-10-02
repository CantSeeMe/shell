/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   f_itoa.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jye <marvin@42.fr>                         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2016/12/08 17:06:42 by jye               #+#    #+#             */
/*   Updated: 2016/12/09 23:00:03 by jye              ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft_printf.h"

static int	sf_dsz(long long int z)
{
	int n;

	n = 0;
	while (z)
	{
		z /= 10;
		++n;
	}
	return (n);
}

int			f_itoa(t_format *c_flag, long long int z, char *buff)
{
	int		nlen;

	if (c_flag->flag & 32 && c_flag->precision < 1 && !z)
		return (0);
	if (!z)
	{
		*buff = 0x30;
		return (1);
	}
	nlen = 0;
	nlen += sf_dsz(z);
	buff += nlen;
	if (z < 0)
		while (z)
		{
			*--buff = 0x30 - (z % 10);
			z /= 10;
		}
	else
		while (z)
		{
			*--buff = 0x30 + (z % 10);
			z /= 10;
		}
	return (nlen);
}
