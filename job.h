/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   job.h                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: root <marvin@42.fr>                        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2017/08/22 10:44:01 by root              #+#    #+#             */
/*   Updated: 2017/09/19 00:37:40 by root             ###   ########.fr       */
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

typedef struct	s_process
{
	pid_t		pid;
	t_command	*c;
	int			qid;
	int			flag;
}				t_process;

typedef struct	s_job
{
	t_lst		*proc;
	int			type;
}				t_job;

extern t_lst	*g_jobs;
extern int		g_laststatus;

t_job			*job_create(t_lst **c);
void			job_exec(t_job *job);

void 			job_setstatus(t_job *job, int status);
char 			*job_getstatus(void);

int				test_execpath(char *c);
char 			**set_envp(void);

void			job_restorefd(t_lst *redir);

void			free_half_parsed_command(t_command *c);
void			free_full_parsed_command(t_command *c);

#endif
