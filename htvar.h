/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   shvar.h                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jye <jye@student.42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2017/06/29 17:23:44 by jye               #+#    #+#             */
/*   Updated: 2017/07/01 16:19:14 by jye              ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef HTVAR_H
# define HTVAR_H

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

int				vhash_init(void);
int				vhash_insert(t_var *var);
void			vhash_pop(char *key);

t_var			*init_var(char *key, char *value, int envp);
int				key_value_string(char *s, char **key, char **value);
int				insert_var(char *s, int envp);
int				boot_var_envp(char **envp);

#endif
