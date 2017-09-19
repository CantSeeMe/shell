/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_getopt_long.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: root <marvin@42.fr>                        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2017/09/14 11:55:45 by root              #+#    #+#             */
/*   Updated: 2017/09/18 23:55:32 by root             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft_getopt_long.h"

#include <string.h>
#include <stdio.h>

#define OPT_ISOPT(opt) ((opt)[0] == '-' && (opt)[1])
#define OPT_ISLONG(opt) (!strncmp((opt), "--", 2) && (opt)[2] != 0)
#define OPT_ISEND(opt) (!strcmp((opt), "--"))

int			optind_;
char		*optarg_;
int			opterr_ = 1;
static char	*pname;

struct s_options *ft_opt_getstruct_long(char *s, struct s_options *longopt)
{
	char	*has_arg;
	size_t	ncmp;

	has_arg = strchr(s, '=');
	if (has_arg)
		optarg_ = has_arg + 1;
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
		if (opterr_)
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

	optarg_ = 0;
	if (optind_ == 0)
	{
		optind_ = 1;
		nextchar = "";
		pname = av[0];
	}
	if (nextchar == NULL)
		return (-1);
	if ((*nextchar == 0 && optind_ >= ac) || OPT_ISEND(av[optind_]))
	{
		nextchar = NULL;
		return (-1);
	}
	else if (*nextchar == 0 && OPT_ISLONG(av[optind_]))
	{
		lopt = ft_opt_getstruct_long(av[optind_] + 2, longopt);
		optind_ += 1;
		if (lopt == NULL)
			return ('?');
		else if (lopt->has_arg == no_arg)
		{
			if (optarg_)
			{
				if (opterr_)
					dprintf(2, "%s: '--%s' doesn't allow an argument\n", pname, lopt->s);
				return ('?');
			}
			if (lopt->f)
				*lopt->f = lopt->val;
			return (lopt->f ? 0 : lopt->val);
		}
		else if (lopt->has_arg == req_arg)
		{
			if (!optarg_)
				optarg_ = av[optind_++];
			if (!optarg_)
			{
				if (opterr_)
					dprintf(2, "%s: '--%s' options requires an argument\n", pname, lopt->s);
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
	else if (*nextchar == 0 && OPT_ISOPT(av[optind_]))
	{
		nextchar = av[optind_] + 1;
		optind_ += 1;
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
			optarg_ = av[optind_++];
		return (*nextchar++);
	}
	else
	{
		if (opterr_)
			dprintf(2, "%s: unrecognized option '%c'\n", pname, *nextchar++);
		return ('?');
	}
}
