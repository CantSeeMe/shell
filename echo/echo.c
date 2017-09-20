/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   echo.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: root <marvin@42.fr>                        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2017/09/18 15:40:24 by root              #+#    #+#             */
/*   Updated: 2017/09/20 17:46:36 by jye              ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft_getopt_long.h"
#include "echo.h"

#include <string.h>
#include <stdlib.h>
#include <stdio.h>

int		ft_echo_get_flag(int ac, char **av)
{
	static struct s_options	lopt[4] = {
		{"null", no_arg, NULL, 'n'},
		{"escape", no_arg, NULL, 'e'},
		{"noescape", no_arg, NULL, 'E'},
		{NULL, 0, NULL, 0}
	};
	char					c;
	int						flag;

	flag = 0;
	while ((c = ft_getopt_long(ac, av, "neE", lopt)) != -1)
	{
		if (c == 'n')
			flag |= null;
		else if (c == 'e')
			flag |= escape;
		else if (c == 'E')
			flag &= ~escape;
		else
			return (-1);
	}
	return (flag);
}

void	ft_echo_process_escape(char *s)
{
	char	*ptr;
	char	*ec;

	ptr = s;
	while (*s)
	{
		if (*s == '\\')
		{
			if (s[1] && (ec = strchr(ECHO_ESCAPE, s[1])) != 0)
			{
				ft_echo_put_escape(&s, &ptr, *ec);
			}
			else
			{
				*ptr++ = *s++;
			}
		}
		else
			*ptr++ = *s++;
	}
	*ptr = 0;
}

void	ft_echo_(int ac, char **av, char end)
{
	int		i;

	i = g_optind_;
	while (i < ac)
	{
		dprintf(1, "%s", av[i++]);
		if (i < ac)
			dprintf(1, " ");
	}
	dprintf(1, "%c", end);
}

int		ft_echo(int ac, char **av, char **envp)
{
	int		flag;
	int		ind;

	(void)envp;
	g_optind_ = 0;
	g_opterr_ = 0;
	flag = ft_echo_get_flag(ac, av);
	ind = g_optind_;
	if (flag & escape)
		while (ind < ac)
			ft_echo_process_escape(av[ind++]);
	ft_echo_(ac, av, flag & null ? 0 : '\n');
	return (0);
}
