/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   htcmd.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jye <jye@student.42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2017/07/01 16:21:36 by jye               #+#    #+#             */
/*   Updated: 2017/10/01 06:12:33 by jye              ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "hashlib.h"
#include "lst.h"
#include "htcmd.h"
#include "libft.h"

#include <dirent.h>
#include <unistd.h>
#include <sys/stat.h>

#include <stdlib.h>
#include <limits.h>

char	*cat_pathfile(char *cur_dir, size_t path_s, char *file)
{
	static char	path[PATH_MAX];
	size_t		f_len;

	f_len = ft_strlen(file);
	if (path_s + f_len > PATH_MAX)
		return (0);
	ft_memcpy(path, cur_dir, path_s);
	path[path_s] = '/';
	ft_memcpy(path + path_s + 1, file, f_len);
	(path_s + f_len)[path + 1] = 0;
	return (path);
}

char	*path_lookup(char *key, char *envp)
{
	char	*ptr;
	char	*path;
	size_t	path_s;

	if (envp == 0)
		return (0);
	while (envp != (char *)1)
	{
		ptr = ft_strchr(envp, ':');
		path_s = ptr - envp;
		path = cat_pathfile(envp, path_s, key);
		if (path && !access(path, X_OK))
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
	n->key = ft_strdup(key);
	if (type)
		n->c = ft_strdup(c);
	else
		n->c = c;
	n->type = type;
	return (n);
}

void	chash_flush(void)
{
	hash_flushtable(g_htext, free_ccsh);
}
