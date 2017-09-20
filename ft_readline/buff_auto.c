/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   buff_auto.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: root <marvin@42.fr>                        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2017/09/03 14:46:41 by root              #+#    #+#             */
/*   Updated: 2017/09/20 19:01:40 by jye              ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "lst.h"
#include "ft_readline.h"
#include "buff_auto.h"

#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include <signal.h>
#include <sys/stat.h>

static char	*cat_broken_file(char *cur_dir, char *file1,
								char *file2)
{
	static char	path[PATH_MAX];
	size_t		cd_len;
	size_t		f1_len;
	size_t		f2_len;

	cd_len = strlen(cur_dir);
	f1_len = strlen(file1);
	f2_len = strlen(file2);
	if (cd_len + f1_len + f2_len >= PATH_MAX)
		return (0);
	memcpy(path, cur_dir, cd_len);
	cd_len[path] = '/';
	memcpy(path + cd_len + 1, file1, f1_len);
	memcpy(path + cd_len + 1 + f1_len, file2, f2_len);
	(cd_len + f1_len + f2_len + 1)[path] = 0;
	return (path);
}

static int	strmatch(char *str, char *match)
{
	int		i;

	i = 0;
	while (str[i] && str[i] == match[i])
		i++;
	return (i);
}

static char	*buff_get_last_part(t_cdir *cdir, size_t als)
{
	char	*s;
	size_t	i;
	int		z;
	int		m;

	s = cdir->file[0] + als;
	m = strlen(s);
	i = 1;
	while (i < cdir->nb_file && m)
	{
		z = strmatch(s, cdir->file[i] + als);
		if (z < m)
			m = z;
		++i;
	}
	s[m] = 0;
	return (s);
}

void		buff_autocomplete_(t_cdir *cdir, char *fold, char *alike)
{
	char		*abs_path;
	char		*ptr_comp;
	struct stat st;
	size_t		n;

	if (cdir->nb_file < 1 || g_buffer.msize == 0)
		return ;
	ptr_comp = buff_get_last_part(cdir, strlen(alike));
	n = strlen(ptr_comp);
	memmove(g_buffer.s + g_buffer.cu + n + (cdir->nb_file == 1),
			g_buffer.s + g_buffer.cu,
			g_buffer.len - g_buffer.cu);
	memcpy(g_buffer.s + g_buffer.cu, ptr_comp, n);
	if (cdir->nb_file == 1)
	{
		if ((abs_path = cat_broken_file(fold, alike, ptr_comp)) == 0)
			return ;
		stat(abs_path, &st);
		g_buffer.s[g_buffer.cu + n] = S_ISDIR(st.st_mode) ? '/' : ' ';
	}
	n += (cdir->nb_file == 1);
	buff_insert(ptr_comp, n);
}
