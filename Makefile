# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: aait-ihi <aait-ihi@student.42.fr>          +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2019/11/29 15:53:22 by aait-ihi          #+#    #+#              #
#    Updated: 2020/01/19 02:48:41 by aait-ihi         ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

NAME = minishell

LIB_FT = libft/libft.a

LIB_READLINE = readline/readline.a

FLAGS = -Wall -Werror -Wextra -g
SRC = cd.c echo.c env.c main.c parsing.c parse_arg.c shellmain.c utilslst.c 21sh_parse.c pipe.c

CC = gcc

OBJ = $(SRC:.c=.o)

all: $(NAME)

$(LIB_READLINE) : lib_readline

lib_readline:
	make -C readline

$(LIB_FT) : lib

lib:
	make -C libft

$(NAME): $(OBJ) $(LIB_FT) $(LIB_READLINE)
	$(CC) $(FLAGS) $(OBJ) -ltermcap $(LIB_FT) $(LIB_READLINE)  -o $(NAME)

%.o: %.c minishell.h
	$(CC) $(FLAGS) -c $< -o $@

clean:
	/bin/rm -f $(OBJ)
	make clean -C libft

fclean: clean
	/bin/rm -f $(NAME)
	make fclean -C libft

re: fclean all
