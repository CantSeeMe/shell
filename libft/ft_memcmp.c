/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_memcmp.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jye <marvin@42.fr>                         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2016/11/04 17:34:30 by jye               #+#    #+#             */
/*   Updated: 2017/10/21 19:01:07 by jye              ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"
#include <stdio.h>
#include <string.h>
#include <stdlib.h>

static int	ft_memcmp_last(void *l_ptr1,
							void *l_ptr2,
							size_t n)
{
	unsigned char *c_ptr1;
	unsigned char *c_ptr2;

	c_ptr1 = (unsigned char *)l_ptr1;
	c_ptr2 = (unsigned char *)l_ptr2;
	while (n--)
	{
		if (*c_ptr1++ != *c_ptr2++)
			return (c_ptr1[-1] - c_ptr2[-1]);
	}
	return (0);
}

static int	ft_memcmp_diff(unsigned long *l_ptr1, unsigned long *l_ptr2)
{
	unsigned char *c_ptr1;
	unsigned char *c_ptr2;

	c_ptr1 = (unsigned char *)l_ptr1;
	c_ptr2 = (unsigned char *)l_ptr2;
	if (*c_ptr1 != *c_ptr2)
		return (*c_ptr1 - *c_ptr2);
	if (*++c_ptr1 != *++c_ptr2)
		return (*c_ptr1 - *c_ptr2);
	if (*++c_ptr1 != *++c_ptr2)
		return (*c_ptr1 - *c_ptr2);
	if (*++c_ptr1 != *++c_ptr2)
		return (*c_ptr1 - *c_ptr2);
	if (*++c_ptr1 != *++c_ptr2)
		return (*c_ptr1 - *c_ptr2);
	if (*++c_ptr1 != *++c_ptr2)
		return (*c_ptr1 - *c_ptr2);
	if (*++c_ptr1 != *++c_ptr2)
		return (*c_ptr1 - *c_ptr2);
	if (*++c_ptr1 != *++c_ptr2)
		return (*c_ptr1 - *c_ptr2);
	return (0);
}

int			ft_memcmp(const void *s1, const void *s2, size_t n)
{
	unsigned char *c_ptr1;
	unsigned char *c_ptr2;
	unsigned long *l_ptr1;
	unsigned long *l_ptr2;

	if (n == 0)
		return (0);
	c_ptr1 = (unsigned char *)s1;
	c_ptr2 = (unsigned char *)s2;
	while (((unsigned long)c_ptr1 & (sizeof(unsigned long) - 1)) && n)
		if (n-- && *c_ptr1++ != *c_ptr2++)
			return (c_ptr1[-1] - c_ptr2[-1]);
	l_ptr1 = (unsigned long *)c_ptr1;
	l_ptr2 = (unsigned long *)c_ptr2;
	if (n > 8)
	{
		while (n > 8)
		{
			if ((*l_ptr1++ ^ *l_ptr2++) != 0)
				return (ft_memcmp_diff(l_ptr1 - 1, l_ptr2 - 1));
			n -= 8;
		}
	}
	return (ft_memcmp_last(l_ptr1, l_ptr2, n));
}
