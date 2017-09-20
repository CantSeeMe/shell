/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   echo1.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jye <jye@student.42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2017/09/20 17:31:24 by jye               #+#    #+#             */
/*   Updated: 2017/09/20 17:32:39 by jye              ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "echo.h"

#include <string.h>

static int	ft_echo_atoc_base(char *s, int ibase, char *cbase)
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

static void	ft_echo_escape_hex(char **s, char **ptr)
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

static void	ft_echo_escape_oct(char **s, char **ptr)
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

void		ft_echo_put_escape(char **s, char **ptr, int ec)
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
