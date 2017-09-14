/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cd.c                                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: root <marvin@42.fr>                        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2017/09/13 15:17:20 by root              #+#    #+#             */
/*   Updated: 2017/09/13 22:23:18 by root             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "htvar.h"
#include <unistd.h>
#include <string.h>
#include <stdlib.h>

char	*chdir_target(int ac, char **av)
{
	if (ac == 1)
		return (vhash_search("HOME"));
	else if (ac == 2 && strcmp(av[1], "-"))
		return (vhash_search("OLDPWD"));
	else if (ac >= 2 && strcmp(av[1], "--"))
	{
		if (ac == 2)
			return (vhash_search("HOME"));
		else
			return (av[2]);
	}
	return (av[1]);
}

int		chdir_test_target(char *target)
{
	int		r;

	if ((r = access(target, F_OK)))
	{} // no such file or directory
	else if ((r = access(target, R_OK)))
	{} // permission denied
	return (r);
}

char	*chdir_get_abs_path(char *old, char *new)
{
	char	*cwdabs;
	size_t	olen;
	size_t	nlen;

	cwdabs = getcwd(0, 0);
	if (cwdabs == NULL)
	{
		olen = strlen(old);
		nlen = strlen(new);
		if ((cwdabs = malloc(olen + nlen + 2)) == 0)
			return (0);
		memcpy(cwdabs, old, olen);
		cwdabs[olen] = '/';
		memcpy(cwdabs + olen + 1, new, nlen + 1);
	}
	return (cwdabs);
}

int		ft_cd(int ac, char **av, char **envp)
{
	char	*target;
	int		st;
	t_var	*cwd;
	t_var	*owd;

	target = chdir_target(ac, av);
	if (chdir_test_target(target))
		return (1);
	st = -(chdir(target));
	if (!st)
	{
		cwd = init_var("PWD=", HTVAR_VAR_ENVP);
		cwd = vhash_insert(cwd);
		owd = init_var("OLDPWD=", HTVAR_VAR_ENVP);
		owd = vhash_insert(owd);
		free(owd->value);
		owd->value = cwd->value;
		cwd->value = chdir_get_abs_path(cwd->value, target);
	}
	return (st);
}
