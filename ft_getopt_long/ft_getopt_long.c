/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_getopt_long.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: root <marvin@42.fr>                        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2017/09/14 11:55:45 by root              #+#    #+#             */
/*   Updated: 2017/09/20 17:45:02 by jye              ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft_getopt_long.h"

#include <string.h>
#include <stdio.h>

#define OPT_ISOPT(opt) ((opt)[0] == '-' && (opt)[1])
#define OPT_ISLONG(opt) (!strncmp((opt), "--", 2) && (opt)[2] != 0)
#define OPT_ISEND(opt) (!strcmp((opt), "--"))

int			g_optind_;
char		*g_optarg_;
int			g_opterr_ = 1;
static char	*g_pname;

struct s_options *ft_opt_getstruct_long(char *s, struct s_options *longopt)
{
	char	*has_arg;
	size_t	ncmp;

	has_arg = strchr(s, '=');
	if (has_arg)
		g_optarg_ = has_arg + 1;
	ncmp = has_arg ? has_arg - s : strlen(s);
	while (longopt->s != NULL)
	{
		if (ncmp == strlen(longopt->s) &&
			!strncmp(s, longopt->s, ncmp))
			break ;
		longopt++;
	}
	if (longopt->s == NULL)
	{
		if (g_opterr_)
			dprintf(2, "%s: unrecognized options '--%*s'\n", pname, ncmp, s);
		return (NULL);
	}
	return (longopt);
}

struct s_options *ft_opt_getstruct(char s, struct s_options *longopt)
{
	while (longopt->s != NULL)
	{
		if (s == longopt->val)
			break ;
		longopt++;
	}
	return (longopt->s ? longopt : NULL);
}


int		ft_getopt_long(int ac, char **av,
					   char *optstr, struct s_options *longopt)
{
	static char	*nextchar;
	struct s_options *lopt;

	g_optarg_ = 0;
	if (g_optind_ == 0)
	{
		g_optind_ = 1;
		nextchar = "";
		g_pname = av[0];
	}
	if (nextchar == NULL)
		return (-1);
	if ((*nextchar == 0 && g_optind_ >= ac) || OPT_ISEND(av[optind_]))
	{
		nextchar = NULL;
		return (-1);
	}
	else if (*nextchar == 0 && OPT_ISLONG(av[g_optind_]))
	{
		lopt = ft_opt_getstruct_long(av[g_optind_] + 2, longopt);
		g_optind_ += 1;
		if (lopt == NULL)
			return ('?');
		else if (lopt->has_arg == no_arg)
		{
			if (g_optarg_)
			{
				if (g_opterr_)
					dprintf(2, "%s: '--%s' doesn't allow an argument\n", g_pname, lopt->s);
				return ('?');
			}
			if (lopt->f)
				*lopt->f = lopt->val;
			return (lopt->f ? 0 : lopt->val);
		}
		else if (lopt->has_arg == req_arg)
		{
			if (!g_optarg_)
				g_optarg_ = av[g_optind_++];
			if (!g_optarg_)
			{
				if (g_opterr_)
					dprintf(2, "%s: '--%s' options requires an argument\n", g_pname, lopt->s);
				return ('?');
			}
			if (lopt->f)
				*lopt->f = lopt->val;
			return (lopt->f ? 0 : lopt->val);
		}
		else
		{
			if (lopt->f)
				*lopt->f = lopt->val;
			return (lopt->f ? 0 : lopt->val);
		}
	}
	else if (*nextchar == 0 && OPT_ISOPT(av[g_optind_]))
	{
		nextchar = av[g_optind_] + 1;
		g_optind_ += 1;
	}
	else if (*nextchar == 0)
	{
		nextchar = NULL;
		return (-1);
	}
	if (strchr(optstr, *nextchar))
	{
		lopt = ft_opt_getstruct(*nextchar, longopt);
		if (lopt->has_arg == req_arg && nextchar[1] == 0)
			g_optarg_ = av[g_optind_++];
		return (*nextchar++);
	}
	else
	{
		if (g_opterr_)
			dprintf(2, "%s: unrecognized option '%c'\n", g_pname, *nextchar++);
		return ('?');
	}
}
