/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   shvar.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jye <jye@student.42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2017/06/29 17:23:12 by jye               #+#    #+#             */
/*   Updated: 2017/09/20 19:46:04 by jye              ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "hashlib.h"
#include "htvar.h"
#include "lst.h"

#include <stdlib.h>
#include <string.h>

int		frag_var(char *s, char **key, char **value)
{
	char	*ptr;
	size_t	klen;

	if ((ptr = strchr(s, '=')) == 0)
		return (1);
	klen = ptr - s;
	*value = ptr[1] == 0 ? 0 : strdup(ptr + 1);
	if ((*key = malloc(klen + 1)) == 0)
	{
		free(*value);
		return (1);
	}
	memcpy(*key, s, klen);
	klen[*key] = 0;
	return (0);
}

char	*defrag_var(char *key, char *value)
{
	size_t	ks;
	size_t	vs;
	char	*s;

	ks = strlen(key);
	vs = value ? strlen(value) : 0;
	if ((s = malloc(ks + vs + 2)) == 0)
		return (0);
	memcpy(s, key, ks);
	s[ks] = '=';
	if (value)
		memcpy(s + ks + 1, value, vs);
	s[ks + vs + 1] = 0;
	return (s);
}

t_var	*init_var(char *s, int envp)
{
	char	*key;
	char	*value;
	t_var	*v;

	if (frag_var(s, &key, &value))
		return ((t_var *)0);
	if ((v = malloc(sizeof(*v))) == 0)
	{
		free(key);
		free(value);
		return ((t_var *)0);
	}
	v->key = key;
	v->value = value;
	if (envp == HTVAR_VAR_ENVP)
	{
		push_lst__(&g_envp, v);
		v->lock_ = g_envp;
		g_envpsize += 1;
	}
	else
		v->lock_ = 0;
	return (v);
}

int		init_htvar(char **envp)
{
	t_var	*v;

	if (vhash_init())
		return (1);
	while (*envp)
	{
		if ((v = init_var(*envp, HTVAR_VAR_ENVP)))
			vhash_insert(v);
		++envp;
	}
	return (0);
}

int		vhash_set_underscore(char *s)
{
	t_bucket	*item;
	t_var		*v;

	if ((item = hash_insert(g_htvar, "_", HT_SEARCH)) == 0)
		return (1);
	if (item->c == 0 && (item->c = init_var("_=", HTVAR_VAR_ENVP)) == 0)
		return (1);
	v = (t_var *)item->c;
	free(v->value);
	v->value = s ? strdup(s) : 0;
	return (0);
}
