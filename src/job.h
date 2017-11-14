/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   job.h                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: root <marvin@42.fr>                        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2017/08/22 10:44:01 by root              #+#    #+#             */
/*   Updated: 2017/11/14 08:20:07 by jye              ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef JOB_H
# define JOB_H

# include "command.h"
# include "lst.h"

# include <unistd.h>
# include <signal.h>
# include <sys/wait.h>

# define JTHANG   	000
# define JTNOHANG 	001
# define JTCOND	  	002
# define JTPIPE   	004

# define JTEXT		000
# define JTBUILTIN	010

# define JT_RUNNING		000
# define JT_DEAD		001
# define JT_BACKGROUND	002
# define JT_FOREGROUND	004
# define JT_SUSPENDED	010

# define JT_DEFAULT_SIZE 1024
# define JT_IS_NOHANG(x) ((x) == (127 << 16))

# define JT_SIGNALED(x) (!WIFEXITED(x))
# define JT_STOPSIG(x)	(WIFSTOPPED((x)) * WSTOPSIG((x)))
# define JT_SIGTERM(x)	(WIFSIGNALED((x)) * WTERMSIG((x)))
# define JT_SIG(x)		(JT_STOPSIG((x)) | JT_SIGTERM(x))
# define JT_EXIT(x)		(WEXITSTATUS(x))

# if defined(_NSIG)
#  define NOSIG _NSIG
# elif defined(NSIG)
#  define NOSIG NSIG
# elif defined(__DARWIN_NSIG)
#  define NOSIG __DARWIN_NSIG
# endif

struct					s_jstat
{
	int		prev;
	int		cur;
	int		jnodecur;
	int		jnodesize;
	int		suspended;
	int		pstat;
	int		exit;
};

typedef struct			s_process
{
	pid_t		pid;
	t_command	*c;
	int			flag;
	int			state;
	int			status;
}						t_process;

typedef struct			s_job
{
	t_lst		*proc;
	int			type;
}						t_job;

extern t_process		**g_jobs;
extern pid_t			g_shgid;
extern pid_t			g_orgid;
extern struct s_jstat	g_js;
extern char				*g_sig_[NOSIG];

t_job					*job_create(t_lst **c);
void					job_exec(t_job *job);

char					*job_getstatus(void);

int						test_execpath(char *c);
int						set_execpath(t_command *c);
void					transmute_av(t_command *c);
char					**set_envp(void);

int						job_rdiropen(t_rd *rd);
int						job_inopen(t_rd *rd);
int						job_outopen(t_rd *rd);

void					job_restorefd(t_lst *redir);
t_lst					*job_openfd(t_lst *redir);
void					job_cond_fork(t_lst **c, int nohang);
void					job_fork_alone(t_lst **c, int nohang);
int						job_pipe_fork(t_lst **c, int nohang);
int						job_exec_process(t_process *p);
void					job_exec(t_job *job);
int						job_wait_control(t_process *proc, int options);
void					free_half_parsed_command(t_command *c);
void					free_full_parsed_command(t_command *c);

int						job_wait_control_(pid_t pid, int options);
void					job_signal_behavior(void (*behavior)(int));
pid_t					job_make_child(int nohang, pid_t pgid);
int						job_init_job_control(void);
void					job_check_jobs(void);
void					job_insert(t_process *proc);
void					job_print_process_status(t_process *proc,
												int qid, char *s);
void					init_sig_string(void);

#endif
