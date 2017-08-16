/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   tokentype.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jye <jye@student.42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2017/06/16 17:11:44 by jye               #+#    #+#             */
/*   Updated: 2017/08/16 04:47:51 by jye              ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "token.h"
#include "hashlib.h"

struct s_tokentype	g_tokentype[] = {
	{"&", ampersand},
	{";", period},
	{"&&", andsym},
	{"||", orsym},
	{"|", pip},
	{">", greater},
	{">>", ggreater},
	{"<", lower},
	{"<<", llower},
	{">&", greater_amp}
};

t_hashtable			*g_httoken;

int					init_tokenizer(void)
{
	int			i;
	t_bucket	*item;

	if ((g_httoken = init_hashtable(48)) == NULL)
		return (1);
	i = 0;
	while (i < MAXTOKEN)
	{
		if ((item = hash_insert(g_httoken,
								g_tokentype[i].s,
								HT_NOSEARCH)) == NULL)
		{
			hash_flushtable(g_httoken, NULL);
			return (1);
		}
		item->c = &g_tokentype[i];
		++i;
	}
	return (0);
}
