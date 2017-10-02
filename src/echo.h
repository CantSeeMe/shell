/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   echo.h                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: root <marvin@42.fr>                        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2017/09/18 15:40:22 by root              #+#    #+#             */
/*   Updated: 2017/09/20 17:33:05 by jye              ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef ECHO_H
# define ECHO_H
# define ECHO_ESCAPE "\\abtnvfr0x"
# define ECHO_OCT "01234567"
# define ECHO_HEX "0123456789ABCDEF"

enum	e_echo_flag
{
	null = 1,
	escape = 2
};

union	u_echo_escape
{
	char val;
	void (*func)();
};

struct	s_echo_escape
{
	char				c;
	int					type;
	union u_echo_escape	e;
};

void	ft_echo_put_escape(char **s, char **ptr, int ec);

int		ft_echo(int ac, char **av, char **envp);

#endif
