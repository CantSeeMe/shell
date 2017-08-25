/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   htcmd.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jye <jye@student.42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2017/07/01 16:21:36 by jye               #+#    #+#             */
/*   Updated: 2017/08/25 06:04:00 by jye              ###   ########.fr       */
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

t_hashtable		*g_htext;
t_hashtable		*g_htbi;

int		chash_init(void)
{
	if ((g_htext = init_hashtable(HT_DEFAULT_BUCKET)) == 0 ||
		(g_htbi = init_hashtable(96)) == 0)
	{
		free(g_htext);
		free(g_htbi);
		return (1);
	}
	else
		return (0);
}

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

void	free_ccsh(t_ccsh *c)
{
	free(c->c);
	free(c->key);
	free(c);
}

t_ccsh	*chash_lookup(char *key, char *envp)
{
	static char	*cpath;

	if (cpath != envp)
	{
		cpath = envp;
		chash_flush();
	}
	return (chash_search(key, cpath));
}

t_ccsh	*chash_search(char *key, char *envp)
{
	char		*cpath;
	t_bucket	*item;
	t_ccsh		*cc;

	if (!(item = hash_search(g_htbi, key)) &&
		!(item = hash_search(g_htext, key)))
//	if (item == 0)
	{
		if ((cpath = path_lookup(key, envp)) == 0)
			return (0);
		if ((cc = init_ccsh(key, cpath, C_SHELL_EXT)) == 0)
			return (0);
		if ((item = hash_insert(g_htext, key, HT_NOSEARCH)) == 0)
		{
			free(cc->c);
			free(cc->key);
			free(cc);
			return (0);
		}
		item->c = cc;
	}
	return (((t_ccsh *)item->c));
}

void	chash_flush(void)
{
	hash_flushtable(g_htext, free_ccsh);
}
