/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cd.c                                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: root <marvin@42.fr>                        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2017/09/13 15:17:20 by root              #+#    #+#             */
/*   Updated: 2017/10/29 17:19:28 by jye              ###   ########.fr       */
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
#include <sys/stat.h>

char	*chdir_target(int ac, char **av)
{
	if (ac == 1)
		return (vhash_search("HOME"));
	else if (ac == 2 && !ft_strcmp(av[1], "-"))
		return (vhash_search("OLDPWD"));
	else if (ac >= 2 && !ft_strcmp(av[1], "--"))
		return (ac == 2 ? vhash_search("HOME") : av[2]);
	return (av[1]);
}

int		chdir_test_target(char *target)
{
	struct stat fs;

	if ((stat(target, &fs)))
	{
		ft_dprintf(2, "%s: cd: %s: No such file or directory\n",
				"21sh", target);
		return (1);
	}
	if ((S_ISDIR(fs.st_mode)) == 0)
	{
		ft_dprintf(2, "%s: cd: %s: Not a directory\n",
				"21sh", target);
		return (1);
	}
	if ((fs.st_mode & (S_IXUSR | S_IXGRP | S_IXOTH)) == 0)
	{
		ft_dprintf(2, "%s: cd: %s: Permission denied\n",
				"21sh", target);
		return (1);
	}
	return (0);
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
		val = getcwd(0, 0);
	if (val == 0)
	{
		val = chdir_get_parent(cwd);
		ft_dprintf(2, "%s: cd: Could not retrieve current working directory,"
				" rolling back to '%s' folder\n", "21sh", val);
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
	if ((target = chdir_target(ac, av)) == 0)
		return (1);
	if (*target != '/')
		target = chdir_guess_target(target, vhash_search("PWD"));
	if (!target || (chdir_test_target(target)))
		return (1);
	chdir(target);
	if ((cwd = chdir_get_wd(CHDIR_PWD)) == 0)
		return (127);
	if ((owd = chdir_get_wd(CHDIR_OLDPWD)) == 0)
		return (0);
	if (cwd->value && !ft_strcmp(cwd->value, target))
		return (0);
	chdir_change_wd(target, owd, cwd);
	return (0);
}
