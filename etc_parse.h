/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   etc_parse.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: root <marvin@42.fr>                        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2017/09/06 19:39:58 by root              #+#    #+#             */
/*   Updated: 2017/09/09 13:46:43 by root             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef ETC_PARSE_H
# define ETC_PARSE_H

#include <stdlib.h>

char	*transmute_alias(char *s);

char	*transmute_dollar(char *s);

int		transmute_special(char *s);
char	*transmute_exp_spec(char *s);

void	givemeback_letter_pls(char *s);
#endif
