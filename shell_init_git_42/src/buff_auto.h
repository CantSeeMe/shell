/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   buff_auto.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jye <jye@student.42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2017/09/20 18:10:48 by jye               #+#    #+#             */
/*   Updated: 2017/09/20 19:12:22 by jye              ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef BUFF_AUTO_H
# define BUFF_AUTO_H

# define MIN_COLUMN_WIDTH 1

# include <unistd.h>
# include <dirent.h>
# include <stdlib.h>

typedef struct	s_cdir
{
	DIR		*cwd;
	size_t	maxlen;
	size_t	nb_file;
	size_t	s_alloc;
	int		bae;
	char	**file;
}				t_cdir;

void			ft_qsort(void **ptr_b, ssize_t size, int (*cmp)());
t_cdir			*buff_get_alike(char *fold, char *alike);
void			buff_show_alike(t_cdir *c);
void			buff_autocomplete_(t_cdir *cdir, char *fold, char *alike);

#endif
