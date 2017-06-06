
#ifndef HASHLIB_H
# define HASHLIB_H

# include <stddef.h>
# include <stdint.h>
# include "lst.h"

# define HASH_TABLE struct s_hash_table
# define BUCKET		struct s_bucket

# define HT_SEARCH		0x0
# define HT_NOSEARCH	0x1

# define HT_DEFAULT_BUCKET 0x1fff

# define HASH_INDEX(k, t, h) ((h = hash_string(k)) & (t)->nbuckets)

struct	s_hash_table
{
	t_lst	**bucket;
	int		nentries;
	int		nbuckets;
};

struct	s_bucket
{
	char		*key;
	void		*item;
	uint64_t	khash;
	int			naccess;
};

HASH_TABLE	*init_hashtable(size_t nbuckets);
HASH_TABLE	*hash_copytable(HASH_TABLE *table);
uint64_t	hash_string(const char *key);
BUCKET		*hash_search(HASH_TABLE *table, char *key);
BUCKET		*hash_insert(HASH_TABLE *table, char *key, int options);
int			hash_resizetable(HASH_TABLE *table, size_t nbuckets);
void		flush_buckets(HASH_TABLE *table, void (*flush)());
void		flush_hashtable(HASH_TABLE *table, void (*flush)());

#endif
