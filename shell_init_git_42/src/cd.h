/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cd.h                                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: root <marvin@42.fr>                        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2017/09/13 15:17:17 by root              #+#    #+#             */
/*   Updated: 2017/09/28 18:29:56 by jye              ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef CD_H
# define CD_H

void	chdir_trim_target(char *target, char *t);
t_var	*chdir_get_pwd(void);

int		ft_cd(int ac, char **av, char **envp);

#endif
