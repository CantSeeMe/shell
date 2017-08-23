/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser.h                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jye <jye@student.42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2017/06/14 20:41:30 by jye               #+#    #+#             */
/*   Updated: 2017/08/23 17:42:28 by root             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef COMMAND_H
# define COMMAND_H

# include "lst.h"
# include "htcmd.h"
# include "error.h"
# define RDF_STDOUT		0x0
# define RDF_STDIN		0x1
# define RDF_FDREDIR	0x2

# define ERROR_UNEXPECTED	0
# define ERROR_EXPECTED 	1

union			u_argv
{
	t_lst	*lav;
	char	**cav;
};

typedef struct	s_command
{
	t_ccsh			cmd;
	char			**envp;
	union u_argv	av;
	t_lst			*redir;
	pid_t			pid;
	int				var_;
	int				ac;
	int				endsym;
}				t_command;

struct			s_fd
{
	char	*s;
	int		o_flag;
	int		fd;
};

typedef struct	s_rdtype
{
	struct s_fd	fd_;
	int			rdtype;
}				t_rdtype;

t_lst			*parse_token(t_lst *token);
t_rdtype		*get_redirection(t_lst **token);

#endif
