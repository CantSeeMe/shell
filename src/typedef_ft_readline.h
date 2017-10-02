/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   typedef_ft_readline.h                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jye <jye@student.42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2017/09/20 19:24:11 by jye               #+#    #+#             */
/*   Updated: 2017/09/20 19:26:54 by jye              ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef TYPEDEF_FT_READLINE_H
# define TYPEDEF_FT_READLINE_H

# include <unistd.h>
# include "lst.h"

typedef struct	s_curs
{
	int		row;
	int		col;
}				t_curs;

typedef struct	s_record
{
	char	*buf;
	ssize_t	bufsize;
	int		start;
	int		action;
}				t_record;

typedef struct	s_buff
{
	char	*s;
	size_t	msize;
	size_t	len;
	size_t	cu;
}				t_buff;

typedef struct	s_chronicle
{
	t_lst		*record;
	char		*s;
	t_buff		cur;
}				t_chronicle;

#endif
