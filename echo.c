/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   echo.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: root <marvin@42.fr>                        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2017/09/18 15:40:24 by root              #+#    #+#             */
/*   Updated: 2017/09/19 01:37:56 by root             ###   ########.fr       */
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
		{"null" , no_arg, NULL,'n'},
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

int		ft_echo_atoc_base(char *s, int ibase, char *cbase)
{
	unsigned long	n;
	char			*c;

	n = 0;
	while (*s && (c = strchr(cbase, *s)))
	{
		n = (n * ibase) + (c - cbase);
		s++;
	}
	return ((char)n);
}

void	ft_echo_escape_hex(char **s, char **ptr)
{
	int		i;
	char	*cp;
	char	*r;
	char	c;

	cp = *s;
	i = 0;
	while (cp[i] && i < 2 && (r = strchr(ECHO_HEX, cp[i])))
		i++;
	if (r == 0 && i == 0)
	{
		*(*ptr)++ = '\\';
		*(*ptr)++ = 'x';
	}
	else
	{
		c = cp[i];
		cp[i] = 0;
		*(*ptr)++ = ft_echo_atoc_base(cp, 16, ECHO_HEX);
		cp[i] = c;
	}
	*s = cp + i;
}

void	ft_echo_escape_oct(char **s, char **ptr)
{
	int		i;
	char	*cp;
	char	*r;
	char	c;

	cp = *s;
	i = 0;
	while (cp[i] && i < 3 && (r = strchr(ECHO_OCT, cp[i])))
		i++;
	if (r == 0 && i == 0)
	{
		*(*ptr)++ = '\\';
		*(*ptr)++ = '0';
	}
	else
	{
		c = cp[i];
		cp[i] = 0;
		*(*ptr)++ = ft_echo_atoc_base(cp, 8, ECHO_OCT);
		cp[i] = c;
	}
	*s = cp + i;

}

void	ft_echo_put_escape(char **s, char **ptr, int ec)
{
	static struct s_echo_escape	esc[10] = {
		{'\\', 0, .e.val = '\\'}, {'a', 0, .e.val = '\a'},
		{'b', 0, .e.val = '\b'}, {'t', 0, .e.val = '\t'},
		{'n', 0, .e.val = '\n'}, {'v', 0, .e.val = '\v'},
		{'f', 0, .e.val = '\f'}, {'r', 0, .e.val = '\r'},
		{'0', 1, .e.func = ft_echo_escape_oct},
		{'x', 1, .e.func = ft_echo_escape_hex}
	};
	int							i;

	i = 0;
	*s += 1;
	while (i < 10)
	{
		if (esc[i].c == ec)
			break ;
		i++;
	}
	*s += 1;
	if (esc[i].type)
		esc[i].e.func(s, ptr);
	else
		*(*ptr)++ = esc[i].e.val;
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

	i = optind_;
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
	optind_ = 0;
	opterr_ = 0;
	flag = ft_echo_get_flag(ac, av);
	ind = optind_;
	if (flag & escape)
		while (ind < ac)
			ft_echo_process_escape(av[ind++]);
	ft_echo_(ac, av, flag & null ? 0 : '\n');
	return (0);
}
