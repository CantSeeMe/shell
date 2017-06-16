/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jye <jye@student.42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2017/06/14 20:41:29 by jye               #+#    #+#             */
/*   Updated: 2017/06/16 19:06:09 by jye              ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <stdlib.h>

#include "hashlib.h"
#include "parser.h"
#include "token.h"

t_lst	*parsetoken(t_lst *token)
{
	syntaxcheck(token);
	processtoken(token);
}
