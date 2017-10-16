#******************************************************************************#
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: jye <jye@student.42.fr>                    +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2017/09/24 01:49:55 by jye               #+#    #+#              #
#    Updated: 2017/10/15 21:39:19 by jye              ###   ########.fr        #
#                                                                              #
#******************************************************************************#

CC       = gcc
CFLAGS   = -Wall -Wextra -Werror -g3
INCLUDE  = -Isrc
LINKLIB  = -lcurses -Lft_printf -lftprintf -Llibft -lft

SRC_PATH = src
OBJ_PATH = obj

SRC_FILE = base.c ft_qsort.c buff_auto.c buff_auto1.c buff_auto2.c buff_motion.c buff_motion1.c buff_motion2.c \
           buff_motion3.c buff_motion4.c buff_motion5.c buff_motion6.c buff_motion7.c buff_motion8.c buff_motion9.c cursor.c \
           ft_readline.c ft_readline1.c ft_readline2.c glob_rl.c putchar_.c \
	       cd.c cd1.c \
	       echo.c echo1.c \
           error.c \
           etc_parse.c etc_parse1.c etc_parse2.c etc_parse3.c \
           exit_.c \
           ft_getopt_long.c ft_getopt_long1.c \
           hashlib.c hashlib1.c \
           htcmd.c htcmd1.c htcmd2.c \
           htvar.c htvar1.c htvar2.c \
           job.c job1.c job2.c job3.c job4.c job5.c job6.c job7.c job8.c job9.c job10.c \
           lst.c \
           parser.c parser1.c parser2.c parser3.c get_redirection.c heretag.c \
           setenv.c \
           freetoken.c token.c tokentype.c \
           unset.c \
	       fg.c bg.c job_.c job_target.c disown.c

OBJ_FILE = $(addprefix $(OBJ_PATH)/, $(addsuffix .o, $(basename $(SRC_FILE))))

NAME     = 21sh

all:      $(NAME)

$(NAME):  $(OBJ_FILE) libft/libft.a ft_printf/libftprintf.a
	$(CC) $(CFLAGS) $(INCLUDE) $(OBJ_FILE) $(LINKLIB) -o $@

libft/libft.a:
	make -C libft

ft_printf/libftprintf.a:
	make -C ft_printf

$(OBJ_PATH)/%.o: $(SRC_PATH)/%.c
	@mkdir -p $(OBJ_PATH)
	$(CC) $(CFLAGS) $(INCLUDE) -c $< -o $@

clean:
	\rm -rf $(OBJ_PATH)
	make -C libft fclean
	make -C ft_printf fclean

fclean: clean
	\rm -rf $(NAME)

re:     fclean all
