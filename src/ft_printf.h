/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_printf.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jye <marvin@42.fr>                         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2016/12/01 00:00:00 by jye               #+#    #+#             */
/*   Updated: 2017/04/22 21:14:26 by jye              ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef FT_PRINTF_H
# define FT_PRINTF_H
# include <stdarg.h>
# include <string.h>
# include "libft.h"
# define FLAG "#-+0 "
# define RESET "#-+0 .*lhjz"
# define BASE "0123456789abcdef"
# define LENGTH "lhjz"
# define SNULL "(null)"
# define BYTE 0x6868
# define WORD 0x68
# define DWORD 0x6c
# define QWORD 0x6c6c
# define MAX 0x6a
# define SIZE 0x7a

/*
**Typedefs
*/

typedef struct		s_format
{
	int				pad;
	int				precision;
	unsigned int	flag;
	unsigned int	length;
	unsigned int	format;
}					t_format;

typedef struct		s_conv
{
	unsigned long int	size;
	void				*content;
	unsigned char		sign;
	char				cpad;
}					t_conv;

/*
**Function prototype printf
*/
int					ft_printf(const char *format, ...);
int					ft_dprintf(const int fd, const char *format, ...);
/*
** Format handler
*/
void				magic(t_format *c_flag, char **format, va_list arg);
/*
** Link to x format conversion
*/
void				magic_conv(t_format *c_flag, va_list arg);
/*
** %s %c
** Preicision ignored
** Defined behavior flag '-'
*/
void				f_char(t_format *c_flag, va_list arg);
void				f_string(t_format *c_flag, va_list arg);
/*
** %C %S %ls %lc
** Defined behavior flag '-'
*/
void				f_wchar(t_format *c_flag, va_list arg);
void				f_wstring(t_format *c_flag, va_list arg);
int					w_char(int wchar, char *stack);
/*
** %d %D %ld %lld %lD %i %li %lli
** padding ignored if precision > padding
** precision overwrite flag 0
** Undefined behavior flag '#'
*/
void				f_sint(t_format *c_flag, va_list arg);
/*
** %u %U %lU %llu %lu
** refer to line 152
** padding ignored if precision > padding
** precision overwrite flag 0
*/
void				f_uint(t_format *c_flag, va_list arg);
/*
** %o %O %lo %lO %llo
** bit shifting >> 3
** refer to line 152
*/
void				f_uoint(t_format *c_flag, va_list arg);
/*
** %x %lx %llx %X %lX %llX
** bit shifting >> 4
** %b
** bit shifting >> 1;
** signed flag ignored aka '+' ' '
** --------------------------------
** %p Defined behavior '-'
*/
void				f_ubint(t_format *c_flag, va_list arg);
void				f_uxint(t_format *c_flag, va_list arg);
void				f_pint(t_format *c_flag, va_list arg);
/*
** %%
*/
void				f_undefined(t_format *c_flag);
/*
** itoa // itoabase
*/
int					f_utoa(t_format *c_flag, unsigned long long int z,
					char *buff);
int					f_itoa(t_format *c_flag, long long int z,
					char *buff);
int					f_utox(t_format *c_flag, unsigned long long int z,
					char *buff);
int					f_utoo(t_format *c_flag, unsigned long long int z,
					char *buff);
int					f_utob(t_format *c_flag, unsigned long long int z,
					char *buff);
/*
** %f %F %a %A %g %G %e %E
*/
/*
**etc function
*/
char				*ft_strchrnul(const char *s, unsigned char c);
void				print_pp(int lpad, char cpad);
void				init_buf(const int fd);
void				write_buf(void *s, unsigned long n);
void				print_buf(void);
size_t				get_len(void);
void				*ft_memallocset(unsigned char c, unsigned long size);

#endif
