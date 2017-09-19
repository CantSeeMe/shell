/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser.h                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jye <jye@student.42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2017/06/14 20:41:30 by jye               #+#    #+#             */
/*   Updated: 2017/09/15 15:41:02 by root             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef COMMAND_H
# define COMMAND_H

# include "lst.h"
# include "htcmd.h"
# include "error.h"
# define RDF_OUT	0x0
# define RDF_IN		0x1
# define RDF_RDIR	0x2

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
	int				var_;
	int				ac;
	int				endsym;
}				t_command;

typedef struct	s_rd
{
	char	*s;
	int		o_flag;
	int		heretag;
	int		fd;
	int		save;
	int		type;
}				t_rd;

t_lst			*parse_token(t_lst *token);
t_rd			*get_redirection(t_lst **token);
void			dummy_redirection(t_lst **token);
void			here_tag(t_rd *rd);

#endif
