/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parse_error.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jye <jye@student.42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2017/06/28 19:20:46 by jye               #+#    #+#             */
/*   Updated: 2017/08/27 00:57:48 by jye              ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

//
#include <stdio.h>
#include "command.h"

void	parse_error(char *to, int errortype)
{
	static const char *e[] = {"%s: unexpected token '%s'\n",
								"%s: expected token near '%s'\n"};
	dprintf(2, e[errortype], PROGRAM_NAME, to);
}

void	exec_error(char *exec, int errortype)
{
	static const char *e[] = {"%s: %s: command not found\n",
							  "%s: %s: No such file or directory\n",
							  "%s: %s: is a directory\n",
							  "%s: %s: Permission denied\n"};
	dprintf(2, e[errortype], PROGRAM_NAME, exec);
}
