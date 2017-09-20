/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   etc_parse.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: root <marvin@42.fr>                        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2017/09/06 19:39:44 by root              #+#    #+#             */
/*   Updated: 2017/09/20 20:55:49 by jye              ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <string.h>
#include <stdio.h>
#include <string.h>

#include "etc_parse.h"
#include "htvar.h"

static char	*transmute_exp_error(char *s, int err)
{
	if (err)
		dprintf(2, "Unexpected end of line.\n");
	free(s);
	return (0);
}

char		*transmute_exp_spec(char *s)
{
	char	*org;
	size_t	offset;
	char	*exp;
	char	last;

	last = 0;
	if (s != (org = maybe_realloc_s1(s, strlen(s), 4)))
		free(s);
	s = org;
	while (transmute_special(s))
	{
		offset = strlen(s) - 1;
		last = offset[s];
		exp = expansion_readline(org, "> ", 2);
		if (exp == 0 && (last == '\'' || last == '"'))
			return (transmute_exp_error(s, 1));
		else if (exp == (char *)-1)
			return (transmute_exp_error(s, 0));
		else
		{
			org = exp;
			s = org + offset;
		}
	}
	return (org);
}
