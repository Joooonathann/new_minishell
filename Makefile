# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: jalbiser <jalbiser@student.42.fr>          +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2024/05/24 11:55:17 by jalbiser          #+#    #+#              #
#    Updated: 2024/09/20 16:41:06 by jalbiser         ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

NAME 		= 	minishell
CC			=	cc
CFLAGS		=	-Wall -Wextra -Werror -g3 $(INCLUDE)
INCLUDE 	=	-I include
READLINE	=	-lreadline
LIBFT		=	./libft/libft.a
MAKE_LIBFT	=	@make -C libft

SRC			=	./src/minishell.c\
				./src/token_list/ft_del_token.c\
				./src/token_list/ft_free_token.c\
				./src/token_list/ft_free_tokens.c\
				./src/token_list/ft_tokenadd_back.c\
				./src/token_list/ft_tokenlast.c\
				./src/token_list/ft_tokennew.c\
				./src/token_list/tokens_split.c\
				./src/token_list/ft_count_tokens.c\
				./src/token_list/ft_count_value_tokens.c\
				./src/parsing/init_ms_token.c\
				./src/parsing/parser.c\
				./src/parsing/tokenizer.c\
				./src/parsing/env_var_expansion.c\
				./src/parsing/handle_quotes.c\
				./src/parsing/add_token_type.c\
				./src/parsing/trime_useless_quotes.c\
				./src/vars/init_vars.c\
				./src/vars/get_vars.c\
				./src/vars/delete_vars.c\
				./src/builtins/commands/echo_command.c\
				./src/builtins/commands/env_command.c\
				./src/builtins/commands/extern_command.c\
				./src/builtins/commands/pwd_command.c\
				./src/builtins/commands/exit_command.c\
				./src/builtins/commands/export_command.c\
				./src/builtins/commands/cd_command.c\
				./src/builtins/commands/unset_command.c\
				./src/builtins/commands/utils/utils_export.c\
				./src/builtins/commands/utils/utils_cd.c\
				./src/builtins/commands/utils/utils_extern.c\
				./src/builtins/commands/utils/exec_redirection.c\
				./src/builtins/commands/utils/exec_system.c\
				./src/builtins/commands/utils/exec_utils.c\
				./src/builtins/commands/utils/exec_func.c\
				./src/builtins/handler_builtins.c\
				./src/builtins/handler_exec.c
				
OBJ			=	${SRC:.c=.o}

all:	$(NAME)

$(LIBFT):
	$(MAKE_LIBFT)

$(NAME): $(OBJ) $(LIBFT)
	$(CC) $(OBJ) $(LIBFT) $(READLINE) -o $(NAME)

clean:
		rm -rf $(OBJ)
		$(MAKE_LIBFT) clean

fclean: clean
		rm -rf $(NAME)
		$(MAKE_LIBFT) fclean

re: fclean all
	$(MAKE_LIBFT) re

.PHONY: all clean fclean re