/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cd.c                                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: root <marvin@42.fr>                        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2017/09/13 15:17:20 by root              #+#    #+#             */
/*   Updated: 2017/09/25 13:23:38 by root             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "htvar.h"
#include "lst.h"
#include "cd.h"
#include "ft_printf.h"
#include "libft.h"

#include <unistd.h>
#include <limits.h>
#include <stdlib.h>

char	*chdir_target(int ac, char **av)
{
	if (ac == 1)
		return (vhash_search("HOME"));
	else if (ac == 2 && !ft_strcmp(av[1], "-"))
		return (vhash_search("OLDPWD"));
	else if (ac >= 2 && !ft_strcmp(av[1], "--"))
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
	{
		ft_dprintf(2, "%s: cd: %s: No such file or directory\n",
				"minishell", target);
	}
	else if ((r = access(target, R_OK)))
	{
		ft_dprintf(2, "%s: cd: %s: Permission denied\n",
				"minishell", target);
	}
	return (r);
}

char	*chdir_get_parent(char *cwd)
{
	char	*ptr;
	char	*last_good;

	last_good = 0;
	if (cwd)
	{
		ptr = cwd + 1;
		while ((ptr = ft_strchr(ptr, '/')))
		{
			*ptr = 0;
			if (access(cwd, F_OK | R_OK))
				break ;
			last_good = ptr;
			*ptr++ = '/';
		}
	}
	else
		return (ft_strdup("/"));
	*last_good = 0;
	ptr = ft_strdup(cwd);
	*last_good = '/';
	return (ptr);
}

char	*chdir_guess_target(char *t, char *cwd)
{
	char		*val;
	static char	target[PATH_MAX];
	size_t		z;

	if (cwd)
		val = access(cwd, F_OK | R_OK) ? getcwd(0, 0) : ft_strdup(cwd);
	else
		val = 0;
	if (val == 0)
	{
		val = chdir_get_parent(cwd);
		ft_dprintf(2, "%s: cd: Could not retrieve current working directory,"
				" rolling back to '%s' folder\n", "minishell", val);
		t = "";
		if (!val)
			return (0);
	}
	z = ft_strlen(val);
	ft_memcpy(target, val, z + 1);
	if (*t != 0)
		chdir_trim_target(target, t);
	free(val);
	return (target);
}

int		ft_cd(int ac, char **av, char **envp)
{
	char		*target;
	t_var		*cwd;
	t_var		*owd;

	(void)envp;
	target = chdir_target(ac, av);
	if (*target != '/')
		target = chdir_guess_target(target, vhash_search("PWD"));
	if (!target || (chdir_test_target(target)))
		return (1);
	chdir(target);
	if ((cwd = init_var("PWD=", HTVAR_VAR_ENVP)) == 0)
		return (127);
	cwd = vhash_insert(cwd);
	if ((owd = init_var("OLDPWD=", HTVAR_VAR_ENVP)))
	{
		owd = vhash_insert(owd);
		free(owd->value);
		owd->value = cwd->value;
	}
	cwd->value = ft_strdup(target);
	return (0);
}
