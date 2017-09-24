/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   etc_parse1.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jye <jye@student.42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2017/09/20 19:56:40 by jye               #+#    #+#             */
/*   Updated: 2017/09/24 11:13:55 by jye              ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "etc_parse.h"
#include "lst.h"
#include "htvar.h"
#include "libft.h"

#include <stdlib.h>
#include <unistd.h>

static char	*str_dollar_malloc(t_lst *tmp, char *sl, size_t slen,
								ssize_t *alloc_)
{
	char					*s;
	struct s_dollar_data	*d_data;

	while (tmp && (s = malloc(*alloc_ + slen + 1)) == 0)
	{
		d_data = (struct s_dollar_data *)tmp->data;
		*alloc_ -= d_data->vsize;
		d_data->vsize = 0;
		d_data->v = 0;
		tmp = tmp->next;
	}
	if (s == 0)
		return (0);
	s[*alloc_ + slen] = 0;
	ft_memcpy(s + *alloc_, sl, slen);
	return (s);
}

static char	*convert_dollar_to_var(t_lst *d, char *sl, ssize_t alloc_)
{
	char					*s;
	struct s_dollar_data	*d_data;
	size_t					slen;

	slen = ft_strlen(sl);
	if ((s = str_dollar_malloc(d, sl, slen, &alloc_)) == 0)
	{
		while (d)
			pop_lst__(&d, free);
		return (s);
	}
	while (d)
	{
		d_data = (struct s_dollar_data *)d->data;
		if (d_data->v)
		{
			alloc_ -= d_data->vsize;
			ft_memcpy(s + alloc_, d_data->v, d_data->vsize);
		}
		slen = d_data->e - d_data->s;
		alloc_ -= slen;
		ft_memcpy(s + alloc_, d_data->s, slen);
		pop_lst__(&d, free);
	}
	return (s);
}

static void	fill_dollar_data(struct s_dollar_data *d_data, char **ptr,
								char **s, ssize_t *alloc_)
{
	char	c;

	d_data->s = *s;
	d_data->e = *ptr;
	*alloc_ += (*ptr)++ - *s;
	*s = (*ptr)++;
	while (**ptr && ft_strchr(WORD_ANCHAR1, **ptr))
		(*ptr)++;
	c = **ptr;
	**ptr = 0;
	d_data->v = vhash_search(*s);
	if (d_data->v)
	{
		d_data->vsize = ft_strlen(d_data->v);
		*alloc_ += d_data->vsize;
	}
	else
		d_data->v = 0;
	**ptr = c;
}

char		*transmute_dollar(char *s)
{
	ssize_t					alloc_;
	char					*ptr;
	struct s_dollar_data	*d_data;
	t_lst					*d;

	alloc_ = 0;
	d = 0;
	while ((ptr = ft_strchr(s, '$')))
	{
		*ptr = 0;
		if ((ptr[1] == 0 || !ft_strchr(WORD_ANCHAR2, ptr[1]) ||
			(d_data = malloc(sizeof(*d_data))) == 0) &&
			(*ptr = -'$'))
			continue ;
		fill_dollar_data(d_data, &ptr, &s, &alloc_);
		if ((push_lst__(&d, d_data)))
		{
			alloc_ -= d_data->vsize + (d_data->e - d_data->s);
			free(d_data);
		}
		s = ptr;
	}
	if (d == 0)
		return (0);
	return (convert_dollar_to_var(d, s, alloc_));
}
