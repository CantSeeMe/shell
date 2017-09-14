/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_getopt_long.h                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: root <marvin@42.fr>                        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2017/09/14 11:56:09 by root              #+#    #+#             */
/*   Updated: 2017/09/14 13:53:37 by root             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef FT_GETOPT_LONG_H
# define FT_GETOPT_LONG_H

enum	e_arg
{
	no_arg,
	req_arg,
	opt_arg
};

struct	s_options
{
	char	*s;
	int		has_arg;
	int		*f;
	int		val;
};

extern int	optind_;
extern char	*optarg_;
extern int	opterr_;

int		ft_getopt_long(int ac, char **av,
					   char *optstring, struct s_options *longopt);

#endif
