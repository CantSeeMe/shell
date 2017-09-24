/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   buff_motion3.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jye <jye@student.42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2017/09/20 19:07:01 by jye               #+#    #+#             */
/*   Updated: 2017/09/24 11:43:17 by jye              ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft_readline.h"
#include "buff_auto.h"
#include "libft.h"

#include <stdlib.h>
#include <curses.h>
#include <term.h>
#include <dirent.h>

void	buff_fill_fold(char **fold, char *p)
{
	char *s;

	s = p;
	while (s > g_buffer.s)
	{
		if (*s == ' ')
			break ;
		s--;
	}
	s += (s != g_buffer.s);
	if (*p == '/' && s == p)
	{
		*fold = ft_strdup("/");
	}
	else if (*p == '/')
	{
		*p = 0;
		*fold = ft_strdup(s);
		*p = '/';
	}
	else
		*fold = ft_strdup(".");
}

void	buff_fill_alike(char **fold, char **alike)
{
	char	*p;
	char	c;

	p = g_buffer.s + g_buffer.cu;
	c = g_buffer.s[g_buffer.cu];
	g_buffer.s[g_buffer.cu] = 0;
	while (p > g_buffer.s)
	{
		if (*p == '/' || *p == ' ')
			break ;
		p--;
	}
	if (p != g_buffer.s + g_buffer.cu && (*p == '/' || *p == ' '))
		*alike = ft_strdup(p + 1);
	else
		*alike = ft_strdup(p);
	g_buffer.s[g_buffer.cu] = c;
	buff_fill_fold(fold, p);
}

void	buff_autocomplete(void)
{
	t_cdir	*cdir;
	char	*fold;
	char	*alike;

	buff_fill_alike(&fold, &alike);
	if ((cdir = buff_get_alike(fold, alike)) == 0)
		return ;
	ft_qsort((void **)cdir->file, cdir->nb_file, ft_strcmp);
	if (g_last_action == buff_autocomplete && cdir->bae != 1)
		buff_show_alike(cdir);
	cdir->bae = cdir->nb_file;
	buff_autocomplete_(cdir, fold, alike);
	free(fold);
	free(alike);
	closedir(cdir->cwd);
}
