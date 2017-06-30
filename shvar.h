/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   shvar.h                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jye <jye@student.42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2017/06/29 17:23:44 by jye               #+#    #+#             */
/*   Updated: 2017/06/30 15:49:33 by root             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef SHVAR_H
# define SHVAR_H

# include "hashlib.h"

extern t_hashtable	*g_htvar;
extern t_lst		*g_envp;
extern t_lst		*g_lockedvar;

typedef struct	s_var
{
	char	*key;
	char	*value;
	int		envp;
	int		lock;
}				t_var;

#endif
