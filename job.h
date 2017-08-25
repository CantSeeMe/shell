/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   job.h                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: root <marvin@42.fr>                        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2017/08/22 10:44:01 by root              #+#    #+#             */
/*   Updated: 2017/08/25 08:08:51 by jye              ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef JOB_H
# define JOB_H

# include "command.h"
# include "lst.h"

# include <unistd.h>

# define JTHANG   00
# define JTNOHANG 01
# define JTCOND	  02
# define JTPIPE   04

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
void	job_grandfather(t_job *c);
void	job_father(t_job *c);
void	job_child_norm(t_command *c);
void	job_father_cond(t_lst *c);
void	job_child_pipe(t_lst *c);
void	job_child_cond(t_lst *c, int endsym);

#endif
