/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   shvar.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jye <jye@student.42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2017/06/29 17:23:12 by jye               #+#    #+#             */
/*   Updated: 2017/06/29 19:03:39 by jye              ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <stdlib.h>
#include "hashlib.h"

#include <string.h>

t_hashtable	g_htvar;

int		insert_shvar(char *s, int envp)
{
	char	*ptr;
	char	*key;
	char	*value;

	ptr = strchr(s, '=');
	*ptr = 0;
	value = NULL;
	if ((key = strdup(s)) == 0 ||
		(value = strdup(ptr + 1)) == 0)
	{
		free(key);
		free(value);
		return (1);
	}
	return (0);
}
