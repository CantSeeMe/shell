/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   job4.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jye <jye@student.42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2017/09/21 20:49:51 by jye               #+#    #+#             */
/*   Updated: 2017/11/11 00:34:58 by jye              ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "lst.h"
#include "command.h"

#include <stdlib.h>
#include <unistd.h>

void	free_half_parsed_command(t_command *c)
{
	t_rd	*rd;

	while (c->av.lav)
		pop_lst__(&c->av.lav, free);
	while (c->redir)
	{
		rd = (t_rd *)c->redir->data;
		free(rd->s);
		if (rd->heretag > 0)
			close(rd->heretag);
		pop_lst__(&c->redir, free);
	}
	free(c);
}

void	free_full_parsed_command(t_command *c)
{
	char	**d;
	t_rd	*rd;

	d = c->av.cav;
	while (*d)
		free(*d++);
	free(c->av.cav);
	d = c->envp;
	while (*d)
		free(*d++);
	free(c->envp);
	while (c->redir)
	{
		rd = (t_rd *)c->redir->data;
		free(rd->s);
		if (rd->heretag > 0)
			close(rd->heretag);
		pop_lst__(&c->redir, free);
	}
	free(c);
}
