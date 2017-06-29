/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   shvar.h                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jye <jye@student.42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2017/06/29 17:23:44 by jye               #+#    #+#             */
/*   Updated: 2017/06/29 19:03:40 by jye              ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef SHVAR_H
# define SHVAR_H

# include "hashlib.h"

extern t_hashtable	g_htvar;

typedef struct	s_shvar
{
	char	*key;
	char	*value;
	int		envp;
}				t_shvar;

#endif
