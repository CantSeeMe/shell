/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cd.h                                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: root <marvin@42.fr>                        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2017/09/13 15:17:17 by root              #+#    #+#             */
/*   Updated: 2017/10/21 18:50:08 by jye              ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef CD_H
# define CD_H

# include "htvar.h"

# define CHDIR_OLDPWD	0x0
# define CHDIR_PWD		0x1

void	chdir_trim_target(char *target, char *t);
t_var	*chdir_get_wd(int wd);
void	chdir_change_wd(char *target, t_var *owd, t_var *cwd);

int		ft_cd(int ac, char **av, char **envp);

#endif
