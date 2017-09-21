/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   job6.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jye <jye@student.42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2017/09/21 20:56:12 by jye               #+#    #+#             */
/*   Updated: 2017/09/21 21:00:19 by jye              ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#define WORD_UCHAR	"ABCDEFGHIJKLMNOPQRSTUVWXYZ"
#define WORD_LCHAR	"abcdefghijklmnopqrstuvwxyz"
#define WORD_SCHAR	"_-"
#define WORD_NCHAR	"1234567890"
#define WORD_ANCHAR	WORD_UCHAR WORD_LCHAR WORD_NCHAR

void	transmute_av(t_command *c)
{
	char	**av;
	char	*dollar;
	t_lst	*z;
	int		i;

	av = malloc(sizeof(*av) * (c->ac + 1));
	z = c->av.lav;
	i = c->ac;
	if (av)
		av[i--] = 0;
	while (z)
	{
		if (av)
		{
			dollar = transmute_dollar((char *)z->data);
			av[i--] = dollar ? dollar : z->data;
			pop_lst__(&z, dollar ? free : 0);
		}
		else
			pop_lst__(&z, free);
	}
	c->av.cav = av;
}

int		check_varname(char *s)
{
	while (*s != '=')
	{
		if (!strchr(WORD_ANCHAR, *s))
			return (0);
		s++;
	}
	return (1);
}

int		set_execpath(t_command *c)
{
	t_ccsh		*z;
	t_var		*v;

	if (strchr(*c->av.cav, '=') && check_varname(*c->av.cav))
	{
		if ((v = init_var(*c->av.cav, HTVAR_VAR_GLOBAL)))
			vhash_insert(v);
		c->var_ = 1;
	}
	else
	{
		c->var_ = 0;
	}
	c->cmd.type = C_SHELL_EXT;
	if (strchr(*(c->av.cav + c->var_), '/'))
		c->cmd.c = *(c->av.cav + c->var_);
	else if ((z = chash_lookup(*(c->av.cav + c->var_), vhash_search("PATH"))))
		c->cmd = *z;
	else
		c->cmd.c = 0;
	return (0);
}

char	**set_envp(void)
{
	int		i;
	t_lst	*env;
	char	**envp;
	t_var	*v;

	if ((envp = malloc(sizeof(char *) * (g_envpsize + 1))) == 0)
		return (0);
	i = g_envpsize;
	envp[i--] = 0;
	env = g_envp;
	while (env)
	{
		v = env->data;
		envp[i--] = defrag_var(v->key, v->value);
		env = env->next;
	}
	return (envp);
}

int		test_execpath(char *c)
{
	struct stat stats;

	if (c == NULL)
		return (1);
	if (stat(c, &stats))
		return (2);
	if (S_ISDIR(stats.st_mode))
		return (3);
	if (!(stats.st_mode & (S_IXUSR | S_IXGRP | S_IXOTH)))
		return (4);
	return (0);
}
