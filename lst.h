/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   lst.h                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jye <jye@student.42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2017/06/14 19:36:45 by jye               #+#    #+#             */
/*   Updated: 2017/06/14 19:39:48 by jye              ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef LST_H
# define LST_H

typedef struct s_lst	t_lst;

struct					s_lst
{
	struct s_lst	*next;
	struct s_lst	*prev;
	void			*data;
};

t_lst					*init_lst__(void *data);
int						push_lst__(t_lst **node, void *data);
void					pop_lst__(t_lst **node, void (*f)());
int						append_lst__(t_lst *node, void *data);
void					swap_lst__(t_lst *high, t_lst *low);

#endif
