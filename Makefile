# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: pmihangy <marvin@42.fr>                    +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2024/09/07 12:06:42 by pmihangy          #+#    #+#              #
#    Updated: 2024/11/11 09:27:21 by pmihangy         ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

## ARGUMENTS
NAME = minishell
SRC = main.c 
SRC_FILES = $(addprefix src/, $(SRC))
UTILS_FILES = error.c
BUILTINS_FILES = echo.c env.c export.c unset.c exit.c pwd.c cd.c
SRC_FILES += $(addprefix src/utils/, $(UTILS_FILES))
CC = cc -g
CFLAGS = -Wall -Wextra #-Werror
RM = rm -rf
INCLUDE = -I./include -I./libft
LIBFT = ./libft
LINKING = -L$(LIBFT) -lft -lreadline

## RULES
all: $(NAME)

$(NAME): $(SRC_FILES)
	make -C $(LIBFT)
	$(CC) $(CFLAGS) $(INCLUDE) $(SRC_FILES) -o $(NAME) $(LINKING)

clean:
	make -C $(LIBFT) clean
	$(RM) *.o

fclean: clean
	make -C $(LIBFT) fclean
	$(RM) $(NAME)

re: fclean all

valgrind:
	valgrind --leak-check=full --show-leak-kinds=all --suppressions=readline.donto ./$(NAME)

.PHONY: all clean fclean re
