/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cd1.c                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jye <jye@student.42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2017/09/20 17:24:19 by jye               #+#    #+#             */
/*   Updated: 2017/09/28 18:29:35 by jye              ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"
#include "htvar.h"

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

t_var	*chdir_get_pwd(void)
{
	t_var	*new;
	t_var	*cwd;

	if ((cwd = vhash_search__("PWD")) == 0)
	{
		if ((new = init_var("PWD=", HTVAR_VAR_ENVP)) == 0)
			return ((t_var *)0);
		if ((cwd = vhash_insert(new)) == 0)
		{
			free(new->key);
			free(new);
			return ((t_var *)0);
		}
	}
	return (cwd);
}
