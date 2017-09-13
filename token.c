/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   token.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jye <jye@student.42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2017/06/14 22:08:31 by jye               #+#    #+#             */
/*   Updated: 2017/09/13 02:24:36 by root             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <stdlib.h>
#include <string.h>
#include "hashlib.h"
#include "token.h"
#include "lst.h"

extern struct s_tokentype	g_tokentype[];
extern t_hashtable			*g_httoken;

static t_symbol				eval_token(char *s)
{
	t_bucket				*item;

	if ((item = hash_search(g_httoken, s)) != NULL)
		return (((struct s_tokentype *)(item->c))->sym);
	while (*s && *s >= 0x30 && *s <= 0x39)
		++s;
	if (*s == 0)
		return (number);
	else
		return (word);
}

static t_token				*init_token(char *s, t_symbol sym)
{
	t_token	*newtoken;

	if ((newtoken = malloc(sizeof(*newtoken))) == (t_token *)0)
		return (NULL);
	newtoken->s = s;
	newtoken->sym = sym;
	return (newtoken);
}

static char					*goto_lastsym(char *s)
{
	char	*e;
	char	c;

	e = s;
	while (*e && strchr(SYMBREAK, *e))
		++e;
	while (e != s)
	{
		c = *e;
		*e = 0;
		if (hash_search(g_httoken, s))
		{
			*e = c;
			return (e);
		}
		*e = c;
		e -= 1;
	}
	return (s);
}

static char					*get_strsymbol(char **line)
{
	char	*s;
	char	*strsym;
	char	*e;
	size_t	symlen;

	if (*(s = *line) == 0)
		return (NULL);
	e = s;
	while (!strchr(NORMPLS, *e))
		++e;
	if ((symlen = e - s) == 0)
	{
		e = goto_lastsym(s);
		if ((symlen = e - s) == 0)
			return (NULL);
	}
	*line = e;
	if ((strsym = malloc(symlen + 1)))
	{
		memcpy(strsym, s, symlen);
		strsym[symlen] = 0;
	}
	return (strsym);
}

t_lst						*tokenize(char *line)
{
	t_lst	*token_item;
	t_lst	*cp;
	t_token	*token;
	char	*s;

	if ((token_item = init_lst__(NULL)) == NULL)
		return (NULL);
	cp = token_item;
	while ((s = get_strsymbol(&line)))
	{
		if ((token = init_token(s, eval_token(s))) == NULL)
		{
			free(s);
			break ;
		}
		token->symbreak = !(strchr(WHITESPACE, *line));
		while (*line && *line == 0x20)
			++line;
		append_lst__(cp, token);
		cp = cp->next;
	}
	pop_lst__(&token_item, NULL);
	return (token_item);
}

void					free_token(t_token *t)
{
	free(t->s);
	free(t);
}
