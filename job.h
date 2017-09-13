/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   job.h                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: root <marvin@42.fr>                        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2017/08/22 10:44:01 by root              #+#    #+#             */
/*   Updated: 2017/09/13 18:08:21 by root             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef JOB_H
# define JOB_H

# include "command.h"
# include "lst.h"

# include <unistd.h>

# define JTHANG   	000
# define JTNOHANG 	001
# define JTCOND	  	002
# define JTPIPE   	004

# define JTEXT		000
# define JTBUILTIN	010

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
void	job_child_pipe(t_lst *c);
void	job_father(t_job *c);

void	job_setlaststatus(int status);
char	*job_retlaststatus(void);

int		job_father_cond(t_lst *c);
int		job_child_norm(t_command *c);
int		job_fork_child(t_lst *c, int endsym);

#endif
