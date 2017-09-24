/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   htcmd.h                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jye <jye@student.42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2017/07/01 16:21:35 by jye               #+#    #+#             */
/*   Updated: 2017/09/24 02:29:39 by jye              ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef HTCMD_H
# define HTCMD_H

# include "hashlib.h"

# define C_SHELL_BUILTIN	0x0
# define C_SHELL_EXT		0x1

typedef int	(*t_builtin)(int ac, char **av, char **envp);

extern t_hashtable	*g_htext;
extern t_hashtable	*g_htbi;

typedef struct		s_ccsh
{
	char			*key;
	int				type;
	void			*c;
}					t_ccsh;

int					chash_init(void);
t_ccsh				*chash_lookup(char *key, char *envp);
t_ccsh				*chash_search(char *key, char *envp);
char				*path_lookup(char *key, char *envp);
t_ccsh				*init_ccsh(char *key, void *c, int type);
void				chash_flush(void);
void				free_ccsh(t_ccsh *z);

#endif
