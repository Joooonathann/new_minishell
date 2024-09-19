/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jalbiser <jalbiser@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/29 10:47:09 by ekrause           #+#    #+#             */
/*   Updated: 2024/09/19 16:12:39 by jalbiser         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef MINISHELL_H
# define MINISHELL_H

# include "../libft/libft.h"
# include <errno.h>
# include <fcntl.h>
# include <limits.h>
# include <readline/history.h>
# include <readline/readline.h>
# include <signal.h>
# include <stdarg.h>
# include <stdio.h>
# include <stdlib.h>
# include <string.h>
# include <sys/types.h>
# include <sys/wait.h>
# include <unistd.h>

# define BOOL unsigned int
# define TRUE 1
# define FALSE 0
# define QUOTE unsigned int
# define SIMPLE 39
# define DOUBLE 34

# define RESET "\033[0m"
# define RED "\033[31m"
# define GREEN "\033[32m"

//	Tokens
typedef enum s_token_type
{
	NONE,
	TYPE_EXECUTABLE,
	TYPE_COMMAND,
	TYPE_ARGUMENT,
	TYPE_OPTION,
	TYPE_REDIRECTION,
	TYPE_S_REDIRECTION,
	TYPE_REDIRECTION_IN,
	TYPE_REDIRECTION_OUT,
	TYPE_PIPE,
}					t_token_type;

typedef struct s_tokens
{
	struct s_tokens	*next;
	struct s_tokens	*prev;
	char			*value;
	unsigned int	quote;
	t_token_type	type;
	char			redirection;
	char			pipe;
}					t_tokens;

typedef struct s_vars
{
	char			*key;
	char			*value;
	BOOL			hide;
	struct s_vars	*next;
}					t_vars;

typedef struct s_minishell
{
	char			*prompt;
	t_vars			*env;
	t_tokens		*tokens;
	t_tokens		**tokens_split;
	t_tokens		*current_tokens;
}					t_minishell;

typedef struct s_lst_cmd
{
	char			*name;
	void			(*func)(t_minishell data);
}					t_lstcmd;

// Token list
void				ft_del_token(t_tokens **token, t_tokens **tokens);
void				ft_free_tokens(t_tokens **tokens);
void				ft_tokenadd_back(t_tokens **tokens, t_tokens *new);
t_tokens			*ft_tokenlast(t_tokens *tokens);
t_tokens			*ft_tokennew(char *value);
int					ft_count_tokens(t_tokens *tokens);
int					ft_count_value_tokens(t_tokens *tokens);
t_tokens			**split_tokens(t_tokens *tokens);

//	PARSING
int					count_quote(char *str, QUOTE quote_type);
char				*init_ms_token(char *str, char *delim);
t_tokens			*parser(char *str, t_vars **env);
void				tokenizer(char **str, t_tokens **tokens);
void				env_var_expansion(t_tokens **tokens, t_vars **env);
char				*add_char_to_str(char *str, char c);
void				handle_quotes(BOOL *in_quote, QUOTE *quote_type, char c,
						char **expanded_value);
void				handle_env_vars(t_tokens *token, int *i, t_vars **env,
						char **expanded_value);
void				handle_special_vars(int *i, char **expanded_value,
						t_vars **env);
void				add_token_type(t_tokens **tokens);
void				trime_useless_quotes(t_tokens **tokens);
char				*add_char_to_str(char *str, char c);
char				*ft_strcat_dynamic(char *dest, char *src);

// VARS - NEW
t_vars				*init_vars(char **envp);
void				add_vars(t_vars *new, t_vars **vars);
t_vars				*get_vars(t_vars **env, char *key);

// BUILTINS / COMMANDS - NEW
void				handler_exec(t_minishell data);
void				handler_builtins(t_minishell data);
void				echo_command(t_minishell data);

#endif