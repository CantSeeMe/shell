/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   env.c                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: root <marvin@42.fr>                        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2017/09/13 22:23:03 by root              #+#    #+#             */
/*   Updated: 2017/11/08 20:32:07 by jye              ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "lst.h"
#include "ft_getopt_long.h"
#include "htvar.h"
#include "htcmd.h"
#include "job.h"
#include "env.h"
#include "libft.h"
#include "ft_printf.h"

#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h>

#define ENV_OPTIONS "iu0"

int		ft_env_flag(int ac, char **av, char *sep)
{
	char					c;
	static struct s_options	optarray[4] = {
		{"ignored", no_arg, NULL, 'i'},
		{"null", no_arg, NULL, '0'},
		{"unset", req_arg, NULL, 'u'},
		{NULL, 0, NULL, 0}
	};

	while ((c = ft_getopt_long(ac, av, ENV_OPTIONS, optarray)) != -1)
	{
		if (c == 'u')
			vhash_pop(g_optarg_);
		else if (c == '0')
			*sep = 0;
		else if (c == 'i')
			ft_env_ignore();
		else
			break ;
	}
	if (c == '?')
		return (1);
	return (0);
}

int		ft_env_newenv(int ind, char **av)
{
	t_var	*v;

	while (av[ind] && ft_strchr(av[ind], '='))
	{
		if ((v = init_var(av[ind], HTVAR_VAR_ENVP)))
			if (!vhash_insert(v))
			{
				free(v->value);
				free(v->key);
				pop_lst__(&g_envp, free);
				g_envpsize -= 1;
			}
		ind++;
	}
	return (ind);
}

void	ft_env_show(char sep)
{
	t_lst	*env;
	t_var	*v;
	char	*s;

	env = g_envp;
	while (env)
	{
		v = env->data;
		s = defrag_var(v->key, v->value);
		if (s)
		{
			ft_dprintf(1, "%s%c", s, sep);
			free(s);
		}
		env = env->next;
	}
	exit(0);
}

void	ft_env_(int ac, char **av)
{
	int		ind;
	char	c;
	char	*path;
	char	*exec_path;

	c = '\n';
	g_optind_ = 0;
	g_opterr_ = 1;
	if (ft_env_flag(ac, av, &c))
		exit(2);
	path = vhash_search("PATH");
	ind = ft_env_newenv(g_optind_, av);
	if (av[ind] == 0)
		ft_env_show(c);
	exec_path = path_lookup(av[ind], path);
	if (!exec_path)
	{
		test_execpath(av[ind]);
		exec_path = av[ind];
	}
	exit(execve(exec_path, av + ind, set_envp()));
}

int		ft_env(int ac, char **av, char **envp)
{
	pid_t	pid;
	int		status;

	pid = job_make_child(0);
	(void)envp;
	if (pid == 0)
		ft_env_(ac, av);
	waitpid(pid, &status, WUNTRACED);
	return (status);
}
