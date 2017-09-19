/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   setenv.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: root <marvin@42.fr>                        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2017/09/14 16:15:14 by root              #+#    #+#             */
/*   Updated: 2017/09/18 23:33:46 by root             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "htvar.h"
#include "env.h"

#include <string.h>
#include <stdlib.h>
#include <stdio.h>

int		ft_setenv(int ac, char **av, char **envp)
{
	int		ind;
	t_var	*v;

	(void)envp;
	if (ac <= 1)
		return (ft_env(0, NULL, 0));
	ind = 1;
	while (ind < ac)
	{
		if (strchr(av[ind], '='))
		{
			if ((v = init_var(av[ind], HTVAR_VAR_ENVP)))
				if (!vhash_insert(v))
				{
					free(v->value);
					free(v->key);
					pop_lst__(&v->lock_, free);
					g_envpsize -= 1;
				}
		}
		else
			dprintf(2, "%s: %s requires an argument\n", "minishell", av[ind]);
		ind++;
	}
	return (0);
}
