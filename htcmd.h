/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   htcmd.h                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jye <jye@student.42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2017/07/01 16:21:35 by jye               #+#    #+#             */
/*   Updated: 2017/07/01 16:53:06 by jye              ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef HTCMD_H
# define HTCMD_H

# include "hashlib.h"

# define C_SHELL_BUILTIN	0x0
# define C_SHELL_EXT		0x1

typedef int	(*t_builtin)(int ac, char **av, char **envp);

extern t_hashtable	*g_htccsh;

union				u_ccty;
{
	char		*path;
	t_builtin	*func;
}

typedef struct		s_ccsh
{
	char			*key;
	union u_ccty	c;
	int				cshell;
}					t_ccsh;

#endif
