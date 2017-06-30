/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   shvar.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jye <jye@student.42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2017/06/29 17:23:12 by jye               #+#    #+#             */
/*   Updated: 2017/06/30 16:56:31 by root             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <stdlib.h>

#include "hashlib.h"
#include "shvar.h"
#include "lst.h"

//
#include <string.h>

t_hashtable		*g_htvar;
t_lst			*g_envp;
t_lst			*g_lockedvar;

int		vhash_init(void)
{
	if ((g_htvar = init_hashtable(HT_DEFAULT_BUCKET)) == 0)
		return (1);
	else
		return (0);
}

int		vhash_insert(t_var *var)
{
	t_bucket	*item;
	t_var		*c;

	if ((item = hash_insert(g_htvar, var->key, HT_SEARCH)) == 0)
		return (1);
	if (item->item)
	{
		c = (t_var *)item->item;
		free(c->value);
		c->value = var->value;
		c->envp = var->envp;
		c->lock = var->lock;
		free(var->key);
		free(var);
	}
	else
	{
		item->item = var;
	}
	return (0);
}

void	vhash_pop(char *key)
{
	t_bucket	*item;
	t_var		*var;

	if ((item = hash_search(g_htvar, key)) == 0)
		return ;
	var = (t_var *)item->item;
	if (var->lock)
	{
		var->envp = 0;
		free(var->value);
		var->value = 0;
		push_lst__(&g_lockedvar, var);
	}
	else
	{
		free(var->key);
		free(var->value);
		hash_popentry(g_htvar, key, free);
	}
}

////////

t_var	*init_var(char *key, char *value, int envp)
{
	t_var	*v;

	if ((v = malloc(sizeof(*v))) == 0)
		return ((t_var *)0);
	v->key = key;
	v->value = value;
	v->envp = envp;
	v->lock = envp;
	return (v);
}
	
int		key_value_string(char *s, char **key, char **value)
{
	char	*ptr;

	if ((ptr = strchr(s, '=')) == 0)
		return (1);
	*ptr = 0;
	*value =  ptr[1] == 0 ? 0 : strdup(ptr + 1);
	if ((*key = strdup(s)) == 0)
	{
		free(*value);
		return (1);
	}
	return (0);
}

int		insert_var(char *s, int envp)
{
	char	*key;
	char	*value;
	t_var	*v;

	if (key_value_string(s, &key, &value))
		return (1);
	if ((v = init_var(key, value, envp)) == 0)
	{
		free(key);
		free(value);
		return (1);
	}
	if (vhash_insert(v))
		return (1);
	if (envp)
		push_lst__(&g_envp, v);
	return (0);
}

int		boot_var_envp(char **envp)
{
	if (vhash_init())
		return (1);
	while (*envp)
	{
		insert_var(*envp, 1);
		++envp;
	}
	return (0);
}
