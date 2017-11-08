/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   hashlib.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jye <jye@student.42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2017/06/06 16:04:05 by jye               #+#    #+#             */
/*   Updated: 2017/11/08 19:37:21 by jye              ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "hashlib.h"
#include "lst.h"

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

t_bucket	*hash_search(t_hashtable *table, char *key)
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
