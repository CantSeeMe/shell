/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   env.c                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: root <marvin@42.fr>                        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2017/09/13 22:23:03 by root              #+#    #+#             */
/*   Updated: 2017/09/19 01:09:03 by root             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h>

#include "lst.h"
#include "ft_getopt_long.h"
#include "htvar.h"
#include "htcmd.h"
#include "job.h"

#define ENV_OPTIONS "iu0"

void	ft_env_ignore(void)
{
	t_var	*c;

	while (g_envp)
	{
		c = g_envp->data;
		vhash_pop(c->key);
	}
}

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
			vhash_pop(optarg_);
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

	while (av[ind] && strchr(av[ind], '='))
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
			dprintf(1, "%s%c", s, sep);
			free(s);
		}
		env = env->next;
	}
	exit(0);
}

void	ft_env_(int ac, char **av)
{
	int		ind;
	t_ccsh	*cmd;
	char	c;
	char	*path;
	char	*exec_path;
	char	**envp;

	c = '\n';
	path = strdup(vhash_search("PATH"));
	optind_ = 0;
	opterr_ = 1;
	if (ft_env_flag(ac, av, &c))
		exit(2);
	ind = ft_env_newenv(optind_, av);
	if (av[ind] == 0)
		ft_env_show(c);
	cmd = chash_lookup(av[ind], path);
	if (!cmd || cmd->type == C_SHELL_BUILTIN)
	{
		test_execpath(av[ind]);
		exec_path = av[ind];
	}
	else
		exec_path = cmd->c;
	envp = set_envp();
	exit(execve(exec_path, av + ind, envp));
}

int		ft_env(int ac, char **av, char **envp)
{
	pid_t	pid;
	int		status;

	pid = fork();
	(void)envp;
	if (pid == 0)
		ft_env_(ac, av);
	waitpid(pid, &status, WUNTRACED);
	return (status);
}
