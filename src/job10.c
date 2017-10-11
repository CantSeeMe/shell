/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   job10.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: root <marvin@42.fr>                        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2017/10/11 13:03:17 by root              #+#    #+#             */
/*   Updated: 2017/10/11 13:06:49 by root             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <signal.h>

const char	*g_sig_[_NSIG];

void	init_sig_string(void)
{
	int	i;

	i = 0;
	while (i < _NSIG)
		g_sig_[i++] = "unknow signal";
	g_sig_[SIGTSTP] = "suspended";
	g_sig_[SIG]
}

void	job_print_process_status(t_process *proc, int status, int qid)
{
	ft_dprintf(1, "[%d] %s%s%s %d %s\n", qid,
			   qid == g_js.cur ? "+ " : "",
			   qid == g_js.prev ? "- " : "",
			   proc->c ? *(proc->c->av.cav + proc->c->var_) : "", proc->pid,
			   sig_[WTERMSIG(status)]);
}
