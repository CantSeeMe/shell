/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   etc_parse3.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jye <jye@student.42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2017/09/20 20:55:59 by jye               #+#    #+#             */
/*   Updated: 2017/09/24 16:28:22 by jye              ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "etc_parse.h"
#include "ft_printf.h"
#include "libft.h"

static int	transmute_dquote(char **s, char **act)
{
	while (**s != '"')
	{
		if (**s == '$' && ft_strchr(WORD_ANCHAR1, (*s)[1]))
		{
			*((unsigned short *)*act) = *((unsigned short *)*s);
			*act += 2;
			*s += 2;
			while (**s && ft_strchr(WORD_ANCHAR2, **s))
				*(*act)++ = *(*s)++;
		}
		else if (**s == '\\' && ft_strchr("$\"\\?", (*s)[1]))
		{
			*s += 1;
			*(*act)++ = -*(*s)++;
		}
		else if (**s == 0)
		{
			*((unsigned int *)*act) = *((unsigned int *)"\xf5\"\0\0");
			return (1);
		}
		else
			*(*act)++ = -*(*s)++;
	}
	return (0);
}

static int	transmute_squote(char **s, char **act)
{
	while (**s != '\'')
	{
		*(*act) = -*(*s)++;
		if (**act == 0)
		{
			*((unsigned int *)*act) = *((unsigned int *)"\xf5'\0\0");
			return (1);
		}
		else
			(*act)++;
	}
	return (0);
}

static int	transmute_backslash(char **s, char **act)
{
	**act = -*++(*s);
	if (**act == 0)
	{
		*(*act) = -'\n';
		(*act)[1] = 0;
		return (1);
	}
	else
		(*act)++;
	return (0);
}

int			transmute_special_(char **s, char **act)
{
	if (**s == '\\')
	{
		if (transmute_backslash(s, act))
			return (1);
		(*s) += 1;
	}
	else if (**s == '\'')
	{
		(*s) += 1;
		if (transmute_squote(s, act))
			return (1);
		(*s) += 1;
	}
	else if (**s == '"')
	{
		(*s) += 1;
		if (transmute_dquote(s, act))
			return (1);
		(*s) += 1;
	}
	return (0);
}

int			transmute_special(char *s)
{
	char	*act;

	act = s;
	while (1)
	{
		while (*s && ft_strchr("'\"\\", *s) == 0)
			*act++ = *s++;
		if (*s == 0)
			break ;
		else if (transmute_special_(&s, &act))
			return (1);
	}
	*act = 0;
	return (0);
}
