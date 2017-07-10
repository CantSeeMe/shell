/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   htcmd.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jye <jye@student.42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2017/07/01 16:21:36 by jye               #+#    #+#             */
/*   Updated: 2017/07/10 15:04:19 by root             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "hashlib.h"
#include "lst.h"

#include <dirent.h>
#include <unistd.h>
#include <sys/stat.h>

#include <string.h>

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
	memcpy(path + cd_len, file, f_len);
	(cd_len + f_len)[path] = 0;
	return (path);
}

char	*path_lookup(char *key, char *envp)
{
	char	*ptr;
	char	*path;

	while (envp != 1)
	{
		ptr = strchr(envp, ':');
		if (ptr)
			*ptr = 0;
		path = cat_pathfile(envp, key);
		if (access(path, X_OK))
			return (path);
		if (ptr)
			ptr = ':';
		envp = ptr + 1;
	}
	return (0);
}

t_ccsh	*init_ccsh(char *key, void *c, int mode)
{
	t_ccsh	*n;

	if ((n = malloc(sizeof(*n))) == 0)
		return (0);
	n->key = strdup(key);
	if (mode)
		n->c.path = strdup(c);
	else
		n->c.func = c;
	return (n);
}

void	free_ccsh(t_ccsh *c)
{
	free(cc->c.path);
	free(cc->key);
	free(cc);
}

char	*cht_lookup(char *key, char *envp)
{
	char		*cpath;
	t_bucket	*item;
	t_ccsh		*cc;

	item = hash_search(g_htext, key);
	if (item == 0)
	{
		if ((cpath = path_lookup(key, envp)) == 0)
			return (0);
		if ((cc = init_ccsh()) == 0)
			return (cpath);
		if ((item = hash_insert(g_htext, key, HT_NOSEARCH)) == 0)
		{
			free(cc->c.path);
			free(cc->key);
			free(cc);
			return (cpath);
		}
		item->c = cc;
	}
	return (((t_ccsh *)item->c)->c.path);
}

void	*chash_flush(void)
{
	hash_flushtable(g_htext, free_ccsh);
}
