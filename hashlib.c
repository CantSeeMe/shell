/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   hashlib.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jye <jye@student.42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2017/06/06 16:04:05 by jye               #+#    #+#             */
/*   Updated: 2017/06/16 17:34:15 by jye              ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "hashlib.h"
#include <stdlib.h>

t_hashtable	*init_hashtable(size_t nbuckets)
{
	t_hashtable	*new_;
	t_lst		**b;
	size_t		i;

	nbuckets += !nbuckets * HT_DEFAULT_BUCKET;
	if ((b = malloc(sizeof(*b) * nbuckets)) == 0)
		return ((t_hashtable *)0);
	if ((new_ = malloc(sizeof(*new_))) == 0)
	{
		free(b);
		return ((t_hashtable *)0);
	}
	i = 0;
	while (i < nbuckets)
		b[i++] = (t_lst *)0;
	new_->nentries = 0;
	new_->nbuckets = nbuckets;
	new_->bucket = b;
	return (new_);
}

uint64_t	hash_string(const char *key)
{
	uint64_t	khash;

	khash = 0;
	while (*key)
	{
		khash *= 281474976710677;
		khash ^= *key | ((*key << 16) << 16);
		key++;
	}
	return (khash);
}

void		hash_swap_entry_maybe(t_lst *high, t_lst *cur)
{
	t_bucket	*hii;
	t_bucket	*cui;

	if (high == 0)
		return ;
	cui = (t_bucket *)cur->data;
	hii = (t_bucket *)high->data;
	if (hii->naccess + 10 < cui->naccess)
		swap_lst__(high, cur);
}

t_bucket		*hash_search(t_hashtable *table, char *key)
{
	t_bucket	*item;
	uint64_t	khash;
	t_lst		*bucket;

	bucket = table->bucket[HT_INDEX(key, table, khash)];
	while (bucket)
	{
		item = (t_bucket *)bucket->data;
		if (item->khash == khash)
		{
			item->naccess += 1;
			hash_swap_entry_maybe(bucket->prev, bucket);
			return (item);
		}
		bucket = bucket->next;
	}
	return ((t_bucket *)0);
}

t_bucket		*hash_insert(t_hashtable *table, char *key, int options)
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
		item->item = NULL;
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

	if ((bucket = table->bucket[(i = HT_INDEX(key, table, khash))]) == 0)
		return (1);
	while (bucket)
	{
		item = bucket->data;
		if (item->khash == khash)
		{
			if (flush)
				flush(item->item);
			pop_lst__(bucket == table->bucket[i] ? table->bucket + i : &bucket, free);
			return (0);
		}
		bucket = bucket->next;
	}
	return (1);
}

void		flush_hashtable(t_hashtable *table, void (*flush)())
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
				flush(item->item);
			pop_lst__(&n, free);
		}
	}
}
