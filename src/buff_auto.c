/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   buff_auto.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: root <marvin@42.fr>                        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2017/09/03 14:46:41 by root              #+#    #+#             */
/*   Updated: 2017/09/24 11:46:53 by jye              ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "lst.h"
#include "ft_readline.h"
#include "buff_auto.h"
#include "libft.h"
#include "ft_printf.h"

#include <stdlib.h>
#include <stdio.h>
#include <limits.h>
#include <signal.h>
#include <sys/stat.h>

static char	*cat_broken_file(char *cur_dir, char *file1,
								char *file2)
{
	static char	path[PATH_MAX];
	size_t		cd_len;
	size_t		f1_len;
	size_t		f2_len;

	cd_len = ft_strlen(cur_dir);
	f1_len = ft_strlen(file1);
	f2_len = ft_strlen(file2);
	if (cd_len + f1_len + f2_len >= PATH_MAX)
		return (0);
	ft_memcpy(path, cur_dir, cd_len);
	cd_len[path] = '/';
	ft_memcpy(path + cd_len + 1, file1, f1_len);
	ft_memcpy(path + cd_len + 1 + f1_len, file2, f2_len);
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
	m = ft_strlen(s);
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
	ptr_comp = buff_get_last_part(cdir, ft_strlen(alike));
	n = ft_strlen(ptr_comp);
	if (cdir->nb_file == 1)
	{
		if ((abs_path = cat_broken_file(fold, alike, ptr_comp)) == 0)
			return ;
		stat(abs_path, &st);
		ptr_comp[n] = S_ISDIR(st.st_mode) ? '/' : ' ';
	}
	n += (cdir->nb_file == 1);
	buff_insert(ptr_comp, n);
}
