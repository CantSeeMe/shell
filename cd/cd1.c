/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cd1.c                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jye <jye@student.42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2017/09/20 17:24:19 by jye               #+#    #+#             */
/*   Updated: 2017/09/20 17:24:45 by jye              ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <string.h>

void	chdir_trim_target(char *target, char *t)
{
	char	*ptr;
	char	*tmp;
	size_t	len;

	while (t != (char *)1)
	{
		ptr = strchr(t, '/');
		if (ptr)
			*ptr = 0;
		if (!strcmp(t, "..") && (tmp = strrchr(target, '/')))
		{
			if (tmp != target)
				*tmp = 0;
		}
		else if (*t && strcmp(t, "."))
		{
			tmp = strchr(target, 0);
			len = strlen(t) + 1;
			if (tmp - target + len >= PATH_MAX)
				break ;
			memcpy(tmp + 1, t, len);
			tmp[0] = '/';
		}
		t = ptr + 1;
	}
}
