/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   job.h                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: root <marvin@42.fr>                        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2017/08/22 10:44:01 by root              #+#    #+#             */
/*   Updated: 2017/08/22 20:24:58 by root             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef JOB_H
# define JOB_H

# include "command.h"
# include "lst.h"

# define JTHANG   0
# define JTNOHANG 1

typedef struct	s_job
{
	pid_t			pid;
	t_lst			*c;
	int				type;
	int				id;
}				t_job;

t_job	*job_create(t_lst **c);
int		job_wake(void);
void	job_check(void);
void	job_exec(t_job *c);
void	job_child_exec(t_job *c);
void	job_father_exec(pid_t pid);

#endif
