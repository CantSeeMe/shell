/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   disown.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jye <jye@student.42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2017/10/15 21:20:10 by jye               #+#    #+#             */
/*   Updated: 2017/10/16 06:41:20 by jye              ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "job.h"
#include "ft_printf.h"
#include "job_target.h"

int		ft_disown(int ac, char **av, char **envp)
{
	int			tar;
	t_process	*p;

	(void)ac;
	(void)av;
	(void)envp;
	tar = job_get_target_job(av);
	if (tar == -1)
	{
		if (av[1])
			ft_dprintf(2, "%s: fg: %s job not found\n", "21sh", av[1]);
		else
			ft_dprintf(2, "%s: fg: no current job\n", "21sh");
		return (1);
	}
	p = g_jobs[tar];
	ft_dprintf(1, "%s: [%d] %s%sjob %d disowned", "21sh", tar + 1,
			tar == g_js.cur ? "+ " : "",
			tar == g_js.prev ? "- " : "",
			p->pid);
	p->state = JT_DEAD;
	return (0);
}
