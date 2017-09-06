/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   buff_auto.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: root <marvin@42.fr>                        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2017/09/03 14:46:41 by root              #+#    #+#             */
/*   Updated: 2017/09/06 19:32:12 by root             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "lst.h"
#include "ft_readline.h"

#include <dirent.h>
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include <signal.h>
#include <sys/stat.h>

#define MIN_COLUMN_WIDTH 1

typedef struct	s_cdir
{
	DIR		*cwd;
	size_t	maxlen;
	size_t	nb_file;
	size_t	s_alloc;
	int		bae;
	char	**file;
}				t_cdir;

void	ft_qsort(void **ptr_b, ssize_t size,
				 int (*cmp)())
{
	void		*tmp_p;
	ssize_t		lo;
	ssize_t		hi;

	if ((hi = size - 2) < 0)
		return ;
	lo = 0;
	while (1)
	{
		while (lo < size - 1 && cmp(ptr_b[lo], ptr_b[size - 1]) < 0)
			lo++;
		while (lo < hi && cmp(ptr_b[hi], ptr_b[size - 1]) > 0)
   			hi--;
		if (lo < hi)
		{
			tmp_p = ptr_b[lo];
			ptr_b[lo] = ptr_b[hi];
			ptr_b[hi] = tmp_p;
		}
		else
			break ;
	}
	tmp_p = ptr_b[lo];
	ptr_b[lo] = ptr_b[size - 1];
	ptr_b[size - 1] = tmp_p;
	ft_qsort(ptr_b, lo, cmp);
	ft_qsort(ptr_b + lo + 1, size - lo - 1, cmp);
}

static int	init_cdir(t_cdir **n)
{
	size_t	z;

	if (*n == 0)
	{
		if ((*n = malloc(sizeof(**n))) == 0)
			return (1);
		memset(*n, 0, sizeof(**n));
		(*n)->s_alloc = 128;
		if (((*n)->file = malloc(sizeof(char *) * 128)) == 0)
		{
			free(*n);
			return (1);
		}
		memset((*n)->file, 0, sizeof(char *) * 128);
	}//	memset((*n)->file, 0, (*n)->nb_file);
	z = (*n)->nb_file;
	while (z--)
	{
		free((*n)->file[z]);
		(*n)->file[z] = 0;
	}
	(*n)->maxlen = 0;
	(*n)->nb_file = 0;
	return (0);
}

static int	cdir_realloc(t_cdir *n)
{
	size_t	i;
	size_t	n_alloc;
	char	**f;

	n_alloc = n->s_alloc * 2;
	if (n_alloc / 2 != n->s_alloc)
		return (1);
	f = n->file;
	if ((n->file = malloc(sizeof(char *) * n_alloc)) == 0)
	{
		n->file = f;
		return (1);
	}
	i = 0;
	memset(n->file + n->s_alloc, 0, n->s_alloc);
	while (i < n->s_alloc)
	{
		n->file[i] = f[i];
		i++;
	}
	n->s_alloc = n_alloc;
	free(f);
	return (0);
}

t_cdir	*buff_get_alike(char *fold, char *alike)
{
	size_t			als;
	char			*s;
	size_t			zz;
	struct dirent	*cwf;
	static t_cdir	*cdir;

	if (init_cdir(&cdir))
		return ((t_cdir *)0);
	if ((cdir->cwd = opendir(fold)) == 0)
		return ((t_cdir *)0);
	als = strlen(alike);
	while ((cwf = readdir(cdir->cwd)))
	{
		if (*alike != '.' && cwf->d_name[0] == '.')
			continue ;
		if (!strncmp(cwf->d_name, alike, als))
		{
			if (cdir->nb_file >= cdir->s_alloc)
			{
				if (cdir_realloc(cdir))
					continue ;
			}
			if ((s = strdup(cwf->d_name)))
			{
				cdir->file[cdir->nb_file++] = s;
				zz = strlen(s);
				if (zz > cdir->maxlen)
					cdir->maxlen = zz;
			}
		}
	}
	return (cdir);
}

void	buff_show_alike(t_cdir *c)
{
	char	s;
	int		z;
	int		skip_;
	int		i;
	int		row;

	if (c->nb_file <= 1)
		return ;
	if (c->nb_file >= 100)
	{
		dprintf(2, "\n\rDisplay %lu possibilities ? (y or n)",
				c->nb_file);
		while (42)
		{
			s = 0;
			read(0, &s, 1);
			if (s == 'n')
			{
				dprintf(2, "\n\r");
				write(2, g_prompt, strlen(g_prompt));
				buff_refresh(0, g_buffer.s, g_buffer.len);
				shift_cursor(g_buffer.len, g_buffer.cu);
				return ;
			}
			else if (s == 'y')
				break ;
			else if (s == 0x3)
			{
				reset_readline();
				return ;
			}
		}
	}
	row = g_winsize.row;
	z = 0;
	c->maxlen += MIN_COLUMN_WIDTH;
	skip_ = g_winsize.col / c->maxlen;
	skip_ = (c->nb_file / skip_) + 1;
	i = 0;
	dprintf(2, "\n\r");
	while (z < (int)c->nb_file)
	{
		while (i < (int)c->nb_file)
		{
			dprintf(2, "%*s", -c->maxlen, c->file[i]);
			i += skip_;
			z++;
		}
		i = (i % skip_) + 1;
		dprintf(2, "\n\r");
		signal(SIGWINCH, reload_line);
		if (row-- < 0)
		{
			s = 0;
			dprintf(2, "--MORE--");
			read(0, &s, 1);
			if (s == 'q')
				break ;
			else if (s == 0x3)
			{
				reset_readline();
				return ;
			}
			dprintf(2, "\e[2K\r");
		}
	}
	write(2, g_prompt, strlen(g_prompt));
	buff_refresh(0, g_buffer.s, g_buffer.len);
	shift_cursor(g_buffer.len, g_buffer.cu);
}

static char	*cat_broken_file(char *cur_dir, char *file1,
							 char *file2)
{
	static char	path[PATH_MAX];
	size_t	cd_len;
	size_t	f1_len;
	size_t	f2_len;

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

static char *buff_get_last_part(t_cdir *cdir, size_t als)
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

void	buff_autocomplete_(t_cdir *cdir, char *fold, char *alike)
{
	char	*abs_path;
	char	*ptr_comp;
	struct stat st;
	size_t	n;

	if (cdir->nb_file < 1)
		return ;
	ptr_comp = buff_get_last_part(cdir, strlen(alike));
	n = strlen(ptr_comp);
	dprintf(3, "%s %d\n", ptr_comp, n);
	memmove(g_buffer.s + g_buffer.cu + n +
			(cdir->nb_file == 1),
			g_buffer.s + g_buffer.cu,
			g_buffer.len - g_buffer.cu);
	memcpy(g_buffer.s + g_buffer.cu,
		   ptr_comp,
		   n);

	if (cdir->nb_file == 1)
	{
		if ((abs_path = cat_broken_file(fold, alike, ptr_comp)) == 0)
			return ;
		stat(abs_path, &st);
		g_buffer.s[g_buffer.cu + n] = S_ISDIR(st.st_mode) ?
			'/':
			' ';
	}
	n += (cdir->nb_file == 1);
	g_buffer.len += n;
	shift_cursor(g_buffer.cu, g_buffer.cu);
	buff_refresh(g_buffer.cu,
				 g_buffer.s + g_buffer.cu,
				 g_buffer.len - g_buffer.cu);
	g_buffer.cu += n;
	shift_cursor(g_buffer.len, g_buffer.cu);
}
