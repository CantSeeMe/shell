/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   token.h                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jye <jye@student.42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2017/06/14 22:08:32 by jye               #+#    #+#             */
/*   Updated: 2017/10/17 20:27:03 by root             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef TOKEN_H
# define TOKEN_H

# include "lst.h"

# define MAXTOKEN 11
# define WHITESPACE " \t\n"
# define SYMBREAK "><&|;"
# define NORMPLS WHITESPACE SYMBREAK

typedef enum	e_symbol
{
	word, number,
	ampersand, period,
	andsym, orsym, pip,
	greater, ggreater, lower, llower, lower_amp, greater_amp
}				t_symbol;

typedef struct	s_token
{
	char		*s;
	t_symbol	sym;
	int			symbreak;
}				t_token;

struct			s_tokentype
{
	char		*s;
	t_symbol	sym;
};

int				init_tokenizer(void);
t_lst			*tokenize(char *line);
void			free_token(t_token *t);

#endif
