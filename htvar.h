/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   shvar.h                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jye <jye@student.42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2017/06/29 17:23:44 by jye               #+#    #+#             */
/*   Updated: 2017/08/25 06:01:53 by jye              ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef HTVAR_H
# define HTVAR_H

# include "hashlib.h"

extern t_lst		*g_envp;
extern int			g_envpsize;

typedef struct	s_var
{
	char	*key;
	char	*value;
	t_lst	*lock_;
}				t_var;

int		vhash_init(void);
int		vhash_insert(t_var *var);
char	*vhash_search(char *key);
t_var	*init_var(char *s, int envp);
int		init_htvar(char **envp);

#endif
