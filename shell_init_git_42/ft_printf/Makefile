#******************************************************************************#
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: jye <marvin@42.fr>                         +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2016/12/02 15:55:18 by jye               #+#    #+#              #
#    Updated: 2017/04/22 20:02:51 by jye              ###   ########.fr        #
#                                                                              #
#******************************************************************************#

# Compiler

CC		=	gcc
CFLAG	=	-Wall -Wextra -Werror

# Directories

SDIR	=	srcs
ODIR	=	obj
HDIR	=	includes

# Files

SRC		=	f_char.c f_string.c f_wstring.c f_wchar.c w_char.c \
			f_sint f_itoa.c f_uint.c f_utoa.c f_ubint.c f_utob.c \
			f_uoint.c f_utox.c f_uxint.c f_utoo.c f_pint.c \
			f_undefined.c ft_strchrnul.c magic_conv.c magic.c ft_printf.c \
			optimizacion.c
OBJ		=	$(addprefix $(ODIR)/, $(addsuffix .o,$(basename $(SRC))))

# Lib name

NAME	=	libftprintf.a

all			:	$(NAME)

.PHONY		:	all clean fclean re

clean		:
	@/bin/rm -rf $(ODIR)

fclean		:	clean
	@/bin/rm -rf $(NAME)

re			:	fclean all

$(NAME)		:	$(OBJ)
	ar -rcs $@ $(OBJ)

$(ODIR)/%.o	:	$(SDIR)/%.c $(LIBFT)
	@/bin/mkdir -p $(ODIR)
	$(CC) -c -o $@ $< $(CFLAG) -I$(HDIR)
