/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   hashlib1.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jye <jye@student.42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2017/09/20 21:56:00 by jye               #+#    #+#             */
/*   Updated: 2017/09/20 21:57:26 by jye              ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "hashlib.h"
#include "lst.h"

#include <stdlib.h>

t_bucket	*hash_insert(t_hashtable *table, char *key, int options)
{
	t_bucket	*item;
	uint64_t	khash;

	item = options == HT_SEARCH ? hash_search(table, key) : (t_bucket *)0;
	if (item == 0)
	{
		if ((item = malloc(sizeof(*item))) == 0)
			return ((t_bucket *)0);
		push_lst__(table->bucket + HT_INDEX(key, table, khash), item);
		item->key = key;
		item->khash = khash;
		item->c = NULL;
		item->naccess = 0;
	}
	return (item);
}

int			hash_resizetable(t_hashtable *table, size_t nbuckets)
{
	t_lst		*old;
	t_lst		**new_;
	t_bucket	*item;
	int			i;

	if (nbuckets == 0)
		return (0);
	if ((new_ = malloc(sizeof(*new_) * nbuckets)) == 0)
		return (1);
	i = 0;
	while (i < table->nbuckets)
	{
		old = table->bucket[i++];
		while (old)
		{
			item = (t_bucket *)old->data;
			pop_lst__(&old, NULL);
			push_lst__(new_ + (item->khash % nbuckets), item);
		}
	}
	free(table->bucket);
	table->bucket = new_;
	table->nbuckets = nbuckets;
	return (0);
}

int			hash_popentry(t_hashtable *table, char *key, void (*flush)())
{
	uint64_t	khash;
	t_lst		*bucket;
	t_bucket	*item;
	int			i;

	if ((bucket =
		table->bucket[(i = HT_INDEX(key, table, khash))]) == 0)
		return (1);
	while (bucket)
	{
		item = bucket->data;
		if (item->khash == khash)
		{
			if (flush)
				flush(item->c);
			pop_lst__(bucket == table->bucket[i] ?
						table->bucket + i : &bucket, free);
			return (0);
		}
		bucket = bucket->next;
	}
	return (1);
}

void		hash_flushtable(t_hashtable *table, void (*flush)())
{
	t_lst		*n;
	t_bucket	*item;
	int			i;

	i = 0;
	while (i < table->nbuckets)
	{
		n = table->bucket[i++];
		while (n)
		{
			item = (t_bucket *)n->data;
			if (flush)
				flush(item->c);
			pop_lst__(&n, free);
		}
	}
}
