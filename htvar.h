/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   shvar.h                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jye <jye@student.42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2017/06/29 17:23:44 by jye               #+#    #+#             */
/*   Updated: 2017/08/22 09:30:17 by root             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef HTVAR_H
# define HTVAR_H

# include "hashlib.h"

extern t_hashtable	*g_htvar;
extern t_lst		*g_envp;

typedef struct	s_var
{
	char	*key;
	char	*value;
	int		envp_;
	t_lst	*lock_;
}				t_var;

int		vhash_init(void);
int		vhash_insert(t_var *var);
char	*vhash_search(char *key);
t_var	*init_var(char *s, int envp);
int		init_htvar(char **envp);

#endif
