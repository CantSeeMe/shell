/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   htcmd.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jye <jye@student.42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2017/07/01 16:21:36 by jye               #+#    #+#             */
/*   Updated: 2017/09/20 21:53:01 by jye              ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "hashlib.h"
#include "lst.h"
#include "htcmd.h"

#include <dirent.h>
#include <unistd.h>
#include <sys/stat.h>

#include <string.h>
#include <stdlib.h>
#include <limits.h>

char	*cat_pathfile(char *cur_dir, char *file)
{
	static char	path[PATH_MAX];
	size_t		cd_len;
	size_t		f_len;

	cd_len = strlen(cur_dir);
	f_len = strlen(file);
	if (cd_len + f_len > PATH_MAX)
		return (0);
	memcpy(path, cur_dir, cd_len);
	path[cd_len] = '/';
	memcpy(path + cd_len + 1, file, f_len);
	(cd_len + f_len)[path + 1] = 0;
	return (path);
}

char	*path_lookup(char *key, char *envp)
{
	char	*ptr;
	char	*path;

	if (envp == 0)
		return (0);
	while (envp != (char *)1)
	{
		ptr = strchr(envp, ':');
		if (ptr)
			*ptr = 0;
		path = cat_pathfile(envp, key);
		if (ptr)
			*ptr = ':';
		if (!access(path, X_OK))
			return (path);
		envp = ptr + 1;
	}
	return (0);
}

t_ccsh	*init_ccsh(char *key, void *c, int type)
{
	t_ccsh	*n;

	if ((n = malloc(sizeof(*n))) == 0)
		return (0);
	n->key = strdup(key);
	if (type)
		n->c = strdup(c);
	else
		n->c = c;
	n->type = type;
	return (n);
}

void	chash_flush(void)
{
	hash_flushtable(g_htext, free_ccsh);
}