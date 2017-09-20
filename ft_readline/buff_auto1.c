/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   buff_auto1.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jye <jye@student.42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2017/09/20 18:40:58 by jye               #+#    #+#             */
/*   Updated: 2017/09/20 19:01:55 by jye              ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "buff_auto.h"
#include <dirent.h>
#include <stdlib.h>
#include <string.h>

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
	}
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

t_cdir		*buff_get_alike(char *fold, char *alike)
{
	size_t			als;
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
		if (strncmp(cwf->d_name, alike, als))
			continue ;
		if (cdir->nb_file >= cdir->s_alloc && cdir_realloc(cdir))
			break ;
		if ((cdir->file[cdir->nb_file] = strdup(cwf->d_name)) == 0)
			continue ;
		cdir->nb_file++;
		if ((zz = strlen(s)) > cdir->maxlen)
			cdir->maxlen = zz;
	}
	return (cdir);
}
