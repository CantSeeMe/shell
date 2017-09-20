/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parse_error.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jye <jye@student.42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2017/06/28 19:20:46 by jye               #+#    #+#             */
/*   Updated: 2017/09/20 21:49:00 by jye              ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <stdio.h>
#include "command.h"

void	exec_error(char *pname, char *exec, int errortype)
{
	static const char *e[] = {"%s: %s: command not found\n",
								"%s: %s: No such file or directory\n",
								"%s: %s: is a directory\n",
								"%s: %s: Permission denied\n"};

	dprintf(2, e[errortype], pname, exec);
}

void	parse_error(char *pname, char *to, int errortype)
{
	static const char *e[] = {"%s: unexpected token '%s'\n",
								"%s: expected token near '%s'\n"};

	dprintf(2, e[errortype], pname, to);
}
