/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   w_char.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jye <marvin@42.fr>                         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2016/12/06 14:59:49 by jye               #+#    #+#             */
/*   Updated: 2016/12/09 15:37:18 by jye              ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft_printf.h"

int		w_char(int wchar, char *stack)
{
	int i;
	int z;

	i = 1;
	z = 0;
	if ((wchar > 0x7f) && (i += 1))
	{
		if ((wchar > 0x7ff) && (i += 1))
		{
			if ((wchar > 0xffff) && (i += 1))
			{
				stack[z++] = 0xf0 | ((wchar >> 18) & 7);
				stack[z++] = 0x80 | ((wchar >> 12) & 63);
			}
			else
				stack[z++] = 0xe0 | ((wchar >> 12) & 15);
			stack[z++] = 0x80 | ((wchar >> 6) & 63);
		}
		else
			stack[z++] = 0xc0 | ((wchar >> 6) & 31);
		stack[z] = 0x80 | (wchar & 63);
	}
	else
		stack[z] = wchar;
	return (i);
}
