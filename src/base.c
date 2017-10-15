/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   base.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jye <jye@student.42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2017/08/16 04:00:33 by jye               #+#    #+#             */
/*   Updated: 2017/10/15 06:26:37 by jye              ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "hashlib.h"
#include "ft_readline.h"
#include "token.h"
#include "command.h"
#include "lst.h"
#include "htcmd.h"
#include "htvar.h"
#include "error.h"
#include "job.h"
#include "etc_parse.h"
#include "libft.h"
#include "ft_printf.h"

#include <stdlib.h>
#include <signal.h>
#include <unistd.h>
#include <termios.h>

int		ft_isatty(int fileno)
{
	struct termios	termios;

	return (tcgetattr(fileno, &termios));
}

void	prompt_shell(void)
{
	char	*s;
	char	*e;
	t_lst	*t;
	t_job	*job;

	t = 0;
	while (1)
	{
		s = ft_readline("21sh> ", ft_strlen("21sh> "));
		if (s == NULL)
			s = ft_strdup("exit");
		if (s != (char *)-1 && s != 0)
		{
			e = transmute_exp_spec(s, 1);
			t = tokenize(e);
			if (e != 0)
				free(e);
			t = parse_token(t);
		}
		job_check_jobs();
		job_signal_behavior(SIG_IGN);
		while (t)
		{
			job = job_create(&t);
			job_exec(job);
		}
		job_signal_behavior(SIG_DFL);
	}
}

void	input_shell(void)
{
	exit(0);
}

int		main(int ac, char **av, char **envp)
{
	(void)ac;
	(void)av;
	(void)envp;
	init_htvar(envp);
	chash_init();
	init_tokenizer();
	job_init_job_control();
	init_sig_string();
	if (!ft_isatty(STDIN_FILENO))
		prompt_shell();
	else
		input_shell();
	setpgid(0, g_orgid);
	return (g_js.pstat);
}
