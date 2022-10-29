# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: masebast <masebast@student.42.fr>          +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2022/09/01 17:02:28 by masebast          #+#    #+#              #
#    Updated: 2022/10/29 17:06:12 by masebast         ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

NAME = minishell
SRCS = *.c
LIBFT = libft/libft.a
FLAGS = -Werror -Wextra -Wall -g -L/usr/include -lreadline -L$$HOME/.brew/opt/readline/lib -I $$HOME/.brew/opt/readline/include/readline

$(NAME):
		make -C libft
		gcc $(FLAGS) $(SRCS) -o $(NAME) $(LIBFT) -ledit

clean:
		rm -rf $(NAME)

fclean:	clean
		make fclean -C libft

all: 	$(NAME)

re:		fclean all