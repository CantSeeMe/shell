/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   setenv.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: root <marvin@42.fr>                        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2017/09/14 16:15:14 by root              #+#    #+#             */
/*   Updated: 2017/09/25 15:54:57 by root             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "htvar.h"
#include "env.h"
#include "libft.h"
#include "ft_printf.h"

#include <stdlib.h>

int		ft_setenv(int ac, char **av, char **envp)
{
	int		i;
	t_var	*v;

	(void)envp;
	if (ac <= 1)
		return (ft_env(1, (char *[]){"env", NULL}, envp));
	i = 1;
	while (i < ac)
	{
		if (ft_strchr(av[i], '='))
		{
			if ((v = init_var(av[i], HTVAR_VAR_ENVP)))
				if (!vhash_insert(v))
				{
					free(v->value);
					free(v->key);
					pop_lst__(&v->lock_, free);
					g_envpsize -= 1;
				}
		}
		else
			ft_dprintf(2, "%s: %s requires an argument\n", "21sh", av[i]);
		i++;
	}
	return (0);
}
