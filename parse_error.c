/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parse_error.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jye <jye@student.42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2017/06/28 19:20:46 by jye               #+#    #+#             */
/*   Updated: 2017/08/19 12:38:11 by root             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

//
#include <stdio.h>
#include "parser.h"

void		parse_error(char *to, int errortype)
{
	static const char *e[] = {"%s: unexpected token '%s'\n",
								"%s: expected token near '%s'\n"};
	dprintf(2, e[errortype], PROGRAM_NAME, to);
}
