/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   job.c                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jye <jye@student.42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2017/07/13 16:07:52 by jye               #+#    #+#             */
/*   Updated: 2017/10/05 12:11:52 by root             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "lst.h"

t_process		**g_jobs;
struct s_jstat	g_js = {-1, -1, 0, 0, 0, 0, 0};
