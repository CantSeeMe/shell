/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   htcmd.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jye <jye@student.42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2017/07/01 16:21:36 by jye               #+#    #+#             */
/*   Updated: 2017/07/01 17:58:19 by jye              ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "hashlib.h"
#include "lst.h"

#include <dirent.h>
#include <unistd.h>
#include <sys/stat.h>

#include <string.h>

#define S_XMASK(x) ((x & S_IXUSR) | (x & S_IXGRP) | (x & S_IXOTH))

t_hashtable		*g_htccsh;

int		chash_insert(t_ccsh *c)
{
	t_bucket	*item;
	t_ccsh		*cmd;

	item = hash_insert(g_htccsh, c->key, HT_NOSEARCH);
	
}

char	*cat_path_file(char *cur_dir, char *file)
{
	char	*path;
	size_t	cd_len;
	size_t	f_len;

	cd_len = strlen(cur_dir);
	f_len = strlen(file);
	if ((path = malloc(cd_len + f_len + 1)) == 0)
		return (0);
	memcpy(path, cur_dir, cd_len);
	memcpy(path + cd_len, file, f_len);
	(cd_len + f_len)[path] = 0;
	return (path);
}

int		insert_direx(char *envp)
{
	DIR				*cwd;
	char			*cat;
	struct dirent	*cwf;
	struct stat		fs;

	if ((cwd = opendir(envp)) == 0)
		return (1);
	while ((cwf = readdir(cwd)))
	{
		if ((cat = cat_path_file(envp, cwf->d_name)) == 0)
			continue ;
		stat(cat, &fs);
		if (S_XMASK(fs.st_mode))
		{
			/* init s_ccsh */
			/* insert s_ccsh, if fail, ignore, proceed to next*/
		}
		free(cat);
	}
	return (0);
}

int		insert_builtin(void)
{

}

int		boot_envp_cmd(char *envp)
{
	char	*ptr;

	/* first step builtin */
	/* second step environment PATH command */
	while (envp != 1)
	{
		ptr = strchr(envp, ':');
		if (ptr)
			*ptr = 0;
		if (insert_direx(envp))
			;
		if (ptr)
			ptr = ':';
		envp = ptr + 1;
	}
	return (1);
}
