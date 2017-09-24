/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cd1.c                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jye <jye@student.42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2017/09/20 17:24:19 by jye               #+#    #+#             */
/*   Updated: 2017/09/24 12:32:23 by jye              ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

#include <limits.h>
#include <stdlib.h>

void	chdir_trim_target(char *target, char *t)
{
	char	*ptr;
	char	*tmp;
	size_t	len;

	while (t != (char *)1)
	{
		ptr = ft_strchr(t, '/');
		if (ptr)
			*ptr = 0;
		if (!ft_strcmp(t, "..") && (tmp = ft_strrchr(target, '/')))
			tmp[tmp == target] = 0;
		else if (*t && ft_strcmp(t, "."))
		{
			tmp = ft_strchr(target, 0);
			tmp -= (target == (tmp - 1));
			len = ft_strlen(t) + 1;
			if (tmp - target + len >= PATH_MAX)
				break ;
			ft_memcpy(tmp + 1, t, len);
			tmp[0] = '/';
		}
		t = ptr + 1;
	}
}
