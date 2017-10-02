/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cd.h                                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: root <marvin@42.fr>                        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2017/09/13 15:17:17 by root              #+#    #+#             */
/*   Updated: 2017/09/28 18:40:00 by root             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef CD_H
# define CD_H

# include "htvar.h"

# define CHDIR_OLDPWD	0x0
# define CHDIR_PWD		0x1

void	chdir_trim_target(char *target, char *t);
t_var	*chdir_get_wd(int wd);

int		ft_cd(int ac, char **av, char **envp);

#endif
