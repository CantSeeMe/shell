/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   f_utox.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jye <marvin@42.fr>                         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2016/12/08 18:00:44 by jye               #+#    #+#             */
/*   Updated: 2016/12/11 16:46:15 by jye              ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft_printf.h"

static int	sf_dsz(unsigned long long int z)
{
	int n;

	n = 0;
	while (z)
	{
		z = z >> 4;
		++n;
	}
	return (n);
}

int			f_utox(t_format *c_flag, unsigned long long int z,
						char *buff)
{
	char	*cbase;
	int		nlen;

	if ((c_flag->flag & 32) && c_flag->precision < 1 && !z)
		return (0);
	cbase = BASE;
	if (!z)
	{
		*buff = 0x30;
		return (1);
	}
	nlen = sf_dsz(z);
	buff += nlen;
	while (z)
	{
		*--buff = cbase[z & 15];
		z = z >> 4;
	}
	return (nlen);
}
