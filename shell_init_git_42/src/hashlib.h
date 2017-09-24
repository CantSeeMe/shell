/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   hashlib.h                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jye <jye@student.42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2017/06/06 15:35:22 by jye               #+#    #+#             */
/*   Updated: 2017/07/10 14:00:41 by root             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef HASHLIB_H
# define HASHLIB_H

# include <stddef.h>
# include <stdint.h>
# include "lst.h"

# define HT_SEARCH		0x0
# define HT_NOSEARCH	0x1

# define HT_DEFAULT_BUCKET 0x2000

# define HT_INDEX(k, t, h) ((h = hash_string(k)) % (t)->nbuckets)

typedef struct	s_hashtable
{
	t_lst	**bucket;
	int		nentries;
	int		nbuckets;
}				t_hashtable;

typedef struct	s_bucket
{
	char		*key;
	void		*c;
	uint64_t	khash;
	int			naccess;
}				t_bucket;

t_hashtable		*init_hashtable(size_t nbuckets);
t_hashtable		*hash_copytable(t_hashtable *table, void *(*dup)());
uint64_t		hash_string(const char *key);
t_bucket		*hash_search(t_hashtable *table, char *key);
t_bucket		*hash_insert(t_hashtable *table, char *key, int options);
int				hash_resizetable(t_hashtable *table, size_t nbuckets);
int				hash_popentry(t_hashtable *table, char *key, void (*flush)());
void			hash_flushtable(t_hashtable *table, void (*flush)());

#endif
