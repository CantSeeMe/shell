/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   error.h                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: root <marvin@42.fr>                        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2017/08/22 08:42:05 by root              #+#    #+#             */
/*   Updated: 2017/08/22 09:28:42 by root             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef ERROR_H
# define ERROR_H

# define PROGRAM_NAME	"minishell"

void	parse_error(char *to, int errortype);
void	exec_error(char *exec, int errortype);

#endif
