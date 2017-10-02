/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   f_utoa.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jye <marvin@42.fr>                         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2016/12/08 17:06:42 by jye               #+#    #+#             */
/*   Updated: 2016/12/10 17:14:06 by jye              ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft_printf.h"

static int	sf_dsz(unsigned long long int z)
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

int			f_utoa(t_format *c_flag, unsigned long long int z, char *buff)
{
	int		nlen;

	if ((c_flag->flag & 32) && !c_flag->precision && !z)
		return (0);
	if (!z)
	{
		*buff = 0x30;
		return (1);
	}
	nlen = sf_dsz(z);
	buff += nlen;
	while (z)
	{
		*--buff = 0x30 + (z % 10);
		z /= 10;
	}
	return (nlen);
}
