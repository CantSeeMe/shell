/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   etc_parse2.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jye <jye@student.42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2017/09/20 20:31:48 by jye               #+#    #+#             */
/*   Updated: 2017/09/24 11:34:49 by jye              ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft_readline.h"
#include "libft.h"

#include <stdlib.h>

char	*maybe_realloc_s1(char *s1, size_t slen, size_t rlen)
{
	char	*ptr_alloc;
	size_t	alloc_;

	alloc_ = ((slen) & ~(DEFAULT_BUFFER_SIZE - 1)) + DEFAULT_BUFFER_SIZE;
	if (slen + rlen > alloc_)
	{
		while (slen + rlen > alloc_)
			alloc_ += DEFAULT_BUFFER_SIZE;
		if ((ptr_alloc = malloc(alloc_)) == 0)
			return (0);
		ft_memcpy(ptr_alloc, s1, slen + 1);
	}
	else
		ptr_alloc = s1;
	return (ptr_alloc);
}

char	*expansion_readline(char *s1, char *pr, size_t prsize)
{
	char	*rl;
	char	*ptr_alloc;
	size_t	slen;
	size_t	rlen;

	rl = ft_readline(pr, prsize);
	if (rl == 0 || rl == (char *)-1)
		return (rl);
	if ((rlen = ft_strlen(rl)) == 0)
	{
		free(rl);
		return (s1);
	}
	slen = ft_strlen(s1);
	if ((ptr_alloc = maybe_realloc_s1(s1, slen, rlen)) == 0)
	{
		free(rl);
		return (0);
	}
	if (ptr_alloc != s1)
		free(s1);
	ft_memcpy(ptr_alloc + slen, rl, rlen);
	free(rl);
	ptr_alloc[slen + rlen] = 0;
	return (ptr_alloc);
}

void	givemeback_letter_pls(char *s)
{
	while (*s)
	{
		if (*s < 0)
			*s = -*s;
		s++;
	}
}
