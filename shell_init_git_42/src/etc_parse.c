/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   etc_parse.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: root <marvin@42.fr>                        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2017/09/06 19:39:44 by root              #+#    #+#             */
/*   Updated: 2017/09/24 11:49:21 by jye              ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft_readline.h"
#include "etc_parse.h"
#include "htvar.h"
#include "ft_printf.h"

#include <stdlib.h>

static char	*transmute_exp_error(char *s, int err)
{
	if (err)
		ft_dprintf(2, "Unexpected end of line.\n");
	free(s);
	return (0);
}

char		*transmute_exp_spec(char *s, int err)
{
	char	*org;
	size_t	offset;
	char	*exp;
	char	last;

	last = 0;
	if (s != (org = maybe_realloc_s1(s, ft_strlen(s), 4)))
		free(s);
	s = org;
	while (transmute_special(s))
	{
		offset = ft_strlen(s) - 1;
		last = offset[s];
		exp = expansion_readline(org, "> ", 2);
		if (exp == 0 && (last == '\'' || last == '"'))
			return (transmute_exp_error(org, err));
		else if (exp == (char *)-1)
			return (transmute_exp_error(org, 0));
		else
		{
			org = exp;
			s = org + offset;
		}
	}
	return (org);
}
