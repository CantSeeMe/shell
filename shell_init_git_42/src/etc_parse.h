/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   etc_parse.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: root <marvin@42.fr>                        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2017/09/06 19:39:58 by root              #+#    #+#             */
/*   Updated: 2017/09/24 02:12:47 by jye              ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef ETC_PARSE_H
# define ETC_PARSE_H

# define WORD_UCHAR	"ABCDEFGHIJKLMNOPQRSTUVWXYZ"
# define WORD_LCHAR	"abcdefghijklmnopqrstuvwxyz"
# define WORD_NCHAR	"1234567890"

# define WORD_ANCHAR1	WORD_UCHAR WORD_LCHAR WORD_NCHAR "_"
# define WORD_ANCHAR2	WORD_UCHAR WORD_LCHAR WORD_NCHAR "_?"

# include "etc_parse.h"
# include <stdlib.h>

struct	s_dollar_data
{
	char	*s;
	char	*e;
	char	*v;
	size_t	vsize;

};

char	*maybe_realloc_s1(char *s1, size_t slen, size_t rlen);

char	*transmute_alias(char *s);
char	*transmute_dollar(char *s);
int		transmute_special(char *s);
char	*transmute_exp_spec(char *s, int err);
void	givemeback_letter_pls(char *s);

#endif
