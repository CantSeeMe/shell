/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   htvar1.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jye <jye@student.42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2017/09/20 19:43:13 by jye               #+#    #+#             */
/*   Updated: 2017/11/08 20:36:55 by jye              ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "hashlib.h"
#include "htvar.h"
#include "lst.h"

#include <stdlib.h>
#include <string.h>

int		vhash_init(void)
{
	return ((g_htvar = init_hashtable(HT_DEFAULT_BUCKET)) == 0);
}

void	vhash_pop(char *key)
{
	t_bucket	*item;
	t_var		*var;

	if ((item = hash_search(g_htvar, key)) == 0)
		return ;
	var = (t_var *)item->c;
	free(var->value);
	if (var->lock_)
	{
		pop_lst__(g_envp == var->lock_ ? &g_envp : &var->lock_, NULL);
		g_envpsize -= 1;
	}
	hash_popentry(g_htvar, key, NULL);
	free(var->key);
	free(var);
}

t_var	*vhash_insert(t_var *var)
{
	t_bucket	*item;
	t_var		*c;

	if ((item = hash_insert(g_htvar, var->key, HT_SEARCH)) == 0)
		return (0);
	if ((c = (t_var *)item->c))
	{
		free(c->value);
		c->value = var->value;
		free(var->key);
		if (var->lock_ && c->lock_)
		{
			pop_lst__(g_envp == var->lock_ ? &g_envp : &var->lock_, NULL);
			g_envpsize -= 1;
		}
		else if (var->lock_ && !c->lock_)
		{
			var->lock_->data = c;
			c->lock_ = var->lock_;
		}
		free(var);
	}
	else
		item->c = var;
	return ((t_var *)item->c);
}

t_var	*vhash_search__(char *key)
{
	t_bucket	*item;

	item = hash_search(g_htvar, key);
	if (item == 0)
		return (0);
	return ((t_var *)item->c);
}

char	*vhash_search(char *key)
{
	t_var	*v;

	v = vhash_search__(key);
	return (v ? v->value : 0);
}
