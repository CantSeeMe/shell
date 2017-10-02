/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strrchr.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jye <marvin@42.fr>                         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2016/11/06 14:22:09 by jye               #+#    #+#             */
/*   Updated: 2017/10/02 14:33:41 by root             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

char	*ft_strrchr(const char *s, unsigned char c)
{
	const char *f;
	const char *p;

	if (c == 0)
		return (ft_strchr(s, 0));
	f = 0;
	while ((p = ft_strchr(s, c)))
	{
		f = p;
		s = p + 1;
	}
	return (f);
}
