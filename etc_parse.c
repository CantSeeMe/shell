/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   etc_parse.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: root <marvin@42.fr>                        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2017/09/06 19:39:44 by root              #+#    #+#             */
/*   Updated: 2017/09/11 14:13:19 by root             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <string.h>
#include <stdio.h>
#include <string.h>

#include "etc_parse.h"
#include "ft_readline.h"
#include "htvar.h"

#define WORD_UCHAR	"ABCDEFGHIJKLMNOPQRSTUVWXYZ"
#define WORD_LCHAR	"abcdefghijklmnopqrstuvwxyz"
#define WORD_NCHAR	"1234567890"

#define WORD_ANCHAR1	WORD_UCHAR WORD_LCHAR WORD_NCHAR "_"
#define WORD_ANCHAR2	WORD_UCHAR WORD_LCHAR WORD_NCHAR "_?"


char	*expansion_readline(char *s1, char *pr, size_t prsize)
{
	char	*rl;
	char	*ptr_alloc;
	size_t	alloc_;
	size_t	slen;
	size_t	rlen;

	rl = ft_readline(pr, prsize);
	if (rl == 0 || rl == (char *)-1)
		return (rl);
	if ((rlen = strlen(rl)) == 0)
	{
		free(rl);
		return (s1);
	}
	slen = strlen(s1);
	alloc_ = (slen & ~(DEFAULT_BUFFER_SIZE - 1)) + DEFAULT_BUFFER_SIZE;
	if (slen + rlen > alloc_)
	{
		while (slen + rlen > alloc_)
			alloc_ += DEFAULT_BUFFER_SIZE;
		if ((ptr_alloc = malloc(alloc_)) == 0)
		{
			free(rl);
			return (0);
		}
		memcpy(ptr_alloc, s1, slen);
		free(s1);
	}
	else
	{
		ptr_alloc = s1;
	}
	memcpy(ptr_alloc + slen, rl, rlen);
	free(rl);
	ptr_alloc[slen + rlen] = 0;
	return (ptr_alloc);
}

int		transmute_special(char *s)
{
	char	*act;

	act = s;
	while (1)
	{
		dprintf(1, "%p\n", s);
		while (*s && strchr("'\"\\", *s) == 0)
			*act++ = *s++;
		if (*s == 0)
			break ;
		else if (*s == '\\')
		{
			*act = -*++s;
			if (*act == 0)
			{
				*act = -'\n';
				act[1] = 0;
				return (1);
			}
			else
				act++;
			s += 1;
		}
		else if (*s == '\'')
		{
			s += 1;
			while (*s != '\'')
			{
				*act = -*s++;
				if (*act == 0)
				{
					*act = -'\n';
					act[1] = '\'';
					act[2] = 0;
					return (1);
				}
				else
					act++;
			}
			s += 1;
		}
		else if (*s == '"')
		{
			s += 1;
			while (*s != '"')
			{
				if (*s == '$' && strchr(WORD_ANCHAR1, s[1]))
				{
					*((unsigned short *)act) = *((unsigned short *)s);
					act += 2;
					s += 2;
					while (*s && strchr(WORD_ANCHAR2, *s))
					{
						*act++ = *s++;
					}
				}
				else if (*s == '\\' && strchr("$\"\\?", s[1]))
				{
					s += 1;
					*act++ = -*s++;
				}
				else if (*s == 0)
				{
					*act = -'\n';
					act[1] = '"';
					act[2] = 0;
					return (1);
				}
				else
				{
					*act++ = -*s++;
				}
			}
			s += 1;
		}
	}
	*act = 0;
	return (0);
}

char	*transmute_exp_spec(char *s)
{
	char	*org;
	size_t	offset;
	char	*exp;
	char	last;

	last = 0;
	org = s;
	while (transmute_special(s))
	{
		offset = strlen(s) - 1;
		last = *(s + offset);
		exp = expansion_readline(org, "> ", 2);
		if (exp == 0 && (last == '\'' || last == '"'))
		{
			dprintf(1, "Unexpected end of line.\n");// unexpected end of line ;
			free(s);
			return (0);
		}
		else if (exp == (char *)-1)
		{
			free(s);
			return (0);
		}
		else
		{
			org = exp;
			s = org + offset;
		}
	}
	return (org);
}

struct	dollar_data
{
	char	*s;
	char	*e;
	char	*v;
	size_t	vsize;

};

char	*convert_dollar_to_var(t_lst *d, char *sl, ssize_t alloc_)
{
	char	*s;
	t_lst	*tmp;
	struct dollar_data *d_data;
	size_t	slen;

	//
	tmp = d;
	slen = strlen(sl);
	while (tmp && (s = malloc(alloc_ + slen + 1)) == 0)
	{
		d_data = tmp->data;
		alloc_ -= d_data->vsize;
		d_data->vsize = 0;
		d_data->v = 0;
		tmp = tmp->next;
	}
	//
	if (s == NULL)
	{
		while (d)
			pop_lst__(&d, free);
		return (s);
	}
	s[alloc_ + slen] = 0;
	memcpy(s + alloc_, sl, slen);
	while (d)
	{
		d_data = d->data;
		if (d_data->v)
		{
			alloc_ -= d_data->vsize;
			memcpy(s + alloc_, d_data->v, d_data->vsize);
		}
		slen = d_data->e - d_data->s;
		alloc_ -= slen;
		memcpy(s + alloc_, d_data->s, slen);
		pop_lst__(&d, free);
	}
	return (s);
}

char	*transmute_dollar(char *s)
{
	ssize_t	alloc_;
	char	*ptr;
	struct	dollar_data	*d_data;
	t_lst	*d;
	char	c;

	alloc_ = 0;
	d = 0;
	while ((ptr = strchr(s, '$')))
	{
		*ptr = 0;
		if (ptr[1] == 0 || !strchr(WORD_ANCHAR2, ptr[1]) ||
			(d_data = malloc(sizeof(*d_data))) == 0)
		{
			*ptr = -'$';
			continue ;
		}
		d_data->s = s;
		d_data->e = ptr;
		alloc_ += ptr++ - s;
		s = ptr++;
		while (*ptr && strchr(WORD_ANCHAR1, *ptr))
			ptr++;
		c = *ptr;
		*ptr = 0;
		d_data->v = vhash_search(s);
		if (d_data->v)
		{
			d_data->vsize = strlen(d_data->v);
			alloc_ += d_data->vsize;
		}
		else
			d_data->v = 0;
		*ptr = c;
		if ((push_lst__(&d, d_data)))
		{
			alloc_ -= d_data->vsize + (d_data->e - d_data->s);
			free(d_data);
		}
		s = ptr;
	}
	if (alloc_ == 0)
		return (0);
	return (convert_dollar_to_var(d, s, alloc_));
}

void	givemeback_letter_pls(char *s)
{
	while (*s)
	{
		if (*s < 0)
			*s = -*s;
		s++;
	}
}
