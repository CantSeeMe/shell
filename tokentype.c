/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   tokentype.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jye <jye@student.42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2017/06/16 17:11:44 by jye               #+#    #+#             */
/*   Updated: 2017/06/16 17:25:58 by jye              ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "token.h"
#include "hashlib.h"

struct s_tokentype	g_tokentype[] = {
	{">", greater},
	{">>", ggreater},
	{"<", lower},
	{"<<", llower},
	{">&", greater_amp},
	{"&", ampersand},
	{"&&", andsym},
	{"|", pipe},
	{"||", orsym},
	{";", period},
	{"=", assign},
	{NULL, -1}
};

t_hashtable			*g_httoken;

int					init_tokenizer(void)
{
	int			i;
	t_bucket	*item;

	if ((g_httoken = init_hashtable(48)) == NULL)
		return (1);
	i = 0;
	while (g_tokentype[i].s != NULL)
	{
		if ((item = hash_insert(g_httoken,
								g_tokentype[i].s,
								HT_NOSEARCH)) == NULL)
		{
			flush_hashtable(g_httoken, NULL);
			return (1);
		}
		item->item = &g_tokentype[i];
		++i;
	}
	return (0);
}
