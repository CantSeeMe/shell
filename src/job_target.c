/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   job_target.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jye <jye@student.42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2017/10/15 21:21:38 by jye               #+#    #+#             */
/*   Updated: 2017/10/16 03:40:47 by jye              ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "job.h"
#include "libft.h"

int		job_test_target(char *s)
{
	int	t;

	t = ft_atoi(s);
	if (t >= g_js.jnodecur)
		return (1);
	else if (g_jobs[t])
	{
		g_js.prev = g_js.cur;
		g_js.cur = t;
		return (0);
	}
	return (1);
}

int		job_get_target_job(char **av)
{
	if (g_js.cur == -1)
		return (-1);
	else if (av[1] == NULL)
		return (g_js.cur);
	else if (av[1][0] == '%')
	{
		if (av[1][1] <= 0x39 && av[1][1] >= 0x30)
			return (job_test_target(av[1] + 1) ?
					-1 : g_js.cur);
		else if (av[1][1] == '+')
			return (g_js.cur);
		else if (av[1][1] == '-' && g_js.prev != -1)
		{
			g_js.prev ^= g_js.cur;
			g_js.cur ^= g_js.prev;
			g_js.prev ^= g_js.cur;
			return (g_js.cur);
		}
	}
	return (-1);
}
