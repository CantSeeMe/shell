/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_qsort.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jye <jye@student.42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2017/09/20 18:13:45 by jye               #+#    #+#             */
/*   Updated: 2017/11/17 15:13:10 by root             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <unistd.h>

void	ft_qsort(void **ptr_b, ssize_t size, int (*cmp)())
{
	void		*tmp_p;
	ssize_t		lo;
	ssize_t		hi;

	if ((hi = size - 2) < 0)
		return ;
	lo = 0;
	while (1)
	{
		while (lo < size - 1 && cmp(ptr_b[lo], ptr_b[size - 1]) < 0)
			lo++;
		while (lo < hi && cmp(ptr_b[hi], ptr_b[size - 1]) >= 0)
			hi--;
		if (lo >= hi)
			break ;
		tmp_p = ptr_b[lo];
		ptr_b[lo] = ptr_b[hi];
		ptr_b[hi] = tmp_p;
	}
	tmp_p = ptr_b[lo];
	ptr_b[lo] = ptr_b[size - 1];
	ptr_b[size - 1] = tmp_p;
	ft_qsort(ptr_b, lo, cmp);
	ft_qsort(ptr_b + lo + 1, size - lo - 1, cmp);
}
