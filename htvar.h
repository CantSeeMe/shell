/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   shvar.h                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jye <jye@student.42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2017/06/29 17:23:44 by jye               #+#    #+#             */
/*   Updated: 2017/09/14 16:12:21 by root             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef HTVAR_H
# define HTVAR_H

# include "hashlib.h"
# include "command.h"

# define HTVAR_SET_PATH		0
# define HTVAR_SET_LASTARGV	1

# define HTVAR_VAR_GLOBAL	0
# define HTVAR_VAR_ENVP		1

extern t_lst		*g_envp;
extern int			g_envpsize;

typedef struct	s_var
{
	char	*key;
	char	*value;
	t_lst	*lock_;
}				t_var;

int		vhash_init(void);
int		vhash_set_underscore(int opt, t_command *c);
t_var	*vhash_insert(t_var *var);
char	*vhash_search(char *key);
t_var	*init_var(char *s, int envp);
char	*defrag_var(char *key, char *value);
int		init_htvar(char **envp);
void	vhash_pop(char *key);

#endif
