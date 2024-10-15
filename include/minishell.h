/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jalbiser <jalbiser@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/29 10:47:09 by ekrause           #+#    #+#             */
/*   Updated: 2024/10/15 22:41:53 by jalbiser         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef MINISHELL_H
# define MINISHELL_H

# include "../libft/libft.h"
# include <errno.h>
# include <fcntl.h>
# include <limits.h>
# include <linux/limits.h>
# include <readline/history.h>
# include <readline/readline.h>
# include <signal.h>
# include <stdarg.h>
# include <stdio.h>
# include <stdlib.h>
# include <string.h>
# include <sys/ioctl.h>
# include <sys/stat.h>
# include <sys/types.h>
# include <sys/wait.h>
# include <unistd.h>

typedef unsigned int	t_bool;
typedef unsigned int	t_quote;

# define TRUE 1
# define FALSE 0
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
}						t_token_type;

typedef struct s_tokens
{
	struct s_tokens		*next;
	struct s_tokens		*prev;
	char				*value;
	unsigned int		quote;
	t_token_type		type;
	char				redirection;
	char				pipe;
}						t_tokens;

typedef struct s_file
{
	char				*name;
	char				*type;
	struct s_file		*next;
}						t_file;

typedef struct s_vars
{
	char				*key;
	char				*value;
	t_bool				hide;
	struct s_vars		*next;
}						t_vars;

typedef struct s_minishell
{
	char				*prompt;
	t_vars				*env;
	t_tokens			*tokens;
	t_tokens			**tokens_split;
	t_tokens			*current_tokens;
	t_file				*files;
	int					output_redirected;
	int					input_redirected;
}						t_minishell;

typedef struct s_lst_cmd
{
	char				*name;
	void				(*func)(t_minishell **data);
}						t_lstcmd;

// Token list
void					ft_del_token(t_tokens **token, t_tokens **tokens);
void					ft_free_tokens(t_tokens **tokens);
void					ft_tokenadd_back(t_tokens **tokens, t_tokens *new);
t_tokens				*ft_tokenlast(t_tokens *tokens);
t_tokens				*ft_tokennew(char *value);
int						ft_count_tokens(t_tokens *tokens);
int						ft_count_value_tokens(t_tokens *tokens);
t_tokens				**split_tokens(t_tokens *tokens);
void					ft_free_tokens_split(t_tokens **tokens);
char					*value_return_expand(t_tokens *token,
							char *expanded_value);

//	PARSING
int						count_quote(char *str, t_quote quote_type);
char					*init_ms_token(char *str, char *delim);
t_tokens				*parser(char *str, t_vars **env);
void					tokenizer(char **str, t_tokens **tokens);
void					env_var_expansion(t_tokens **tokens, t_vars **env);
char					*add_char_to_str(char *str, char c);
void					handle_quotes(t_bool *in_quote, t_quote *quote_type,
							char c, char **expanded_value);
void					handle_env_vars(t_tokens *token, int *i, t_vars **env,
							char **expanded_value);
void					handle_special_vars(int *i, char **expanded_value,
							t_vars **env);
void					add_token_type(t_tokens **tokens);
void					trime_useless_quotes(t_tokens **tokens);
char					*add_char_to_str(char *str, char c);
char					*ft_strcat_dynamic(char *dest, char *src);
void					ft_error(int count, ...);

// VARS - NEW
t_vars					*init_vars(char **envp);
void					add_vars(t_vars *new, t_vars **vars);
t_vars					*get_vars(t_vars **env, char *key);
void					env_command(t_minishell **data);
void					extern_command(t_minishell **data);
void					update_vars(t_vars **env, char *key, char *value);
void					delete_all_vars(t_vars **vars);
int						delete_vars(t_vars **env, t_vars *delete);
int						exist_masked(t_vars *env, char *key);
int						exist_vars(t_vars *env, char *key);
void					expand_var_heredoc(char **str, t_vars **env);

// BUILTINS / COMMANDS - NEW
void					handler_exec(t_minishell **data);
void					handler_builtins(t_minishell **data);
void					echo_command(t_minishell **data);
void					pwd_command(t_minishell **data);
void					exit_command(t_minishell **data);
void					export_command(t_minishell **data);
void					cd_command(t_minishell **data);
void					unset_command(t_minishell **data);

// UTILS
void					insert_sorted(t_vars **sorted, t_vars *new);
t_vars					*sort_vars(t_vars *vars);
t_vars					*dup_env(t_vars *vars);
void					print_env(t_vars *vars);
int						calculate_size_export(char *str, t_bool add);
int						handle_home_path(t_minishell **data, char **path,
							t_tokens *token);
void					handle_tilde(t_minishell **data, char **path);
void					handle_chdir_error(char *path, t_minishell **data);
int						get_count_vars(t_vars *env);
char					**get_env(t_vars *env);
char					*build_full_path(char *dir, char *cmd);
void					fetch_redirection(t_minishell **data, t_tokens *tokens);
t_file					*get_files(t_tokens *tokens);
t_file					*new_file(char *value, char *type);
void					add_file(t_file **tokens, t_file *new);
t_tokens				*get_tokens_new(t_tokens *tokens);
int						count_tokens_split(t_tokens **tokens);
int						nofork_command(t_tokens *tokens);
void					clean_process(t_minishell **data, t_bool env,
							t_bool data_free);
void					execute_process(t_minishell **data);
void					up_shlvl(t_vars **env);
void					write_to_heredoc_pipe(int fd, const char *line);
void					close_heredoc_pipe(int fd);
void					read_heredoc_lines(t_minishell **data,
							int heredoc_pipe);
void					handle_heredoc(t_minishell **data);
int						is_on_redirection(t_tokens *tokens);
char					*compose_tokens(t_tokens *tokens);
void					reparse(t_minishell **data);
int						is_last_heredoc(t_minishell **data);
void					free_current(t_tokens **tokens);
void					free_split_tokens(t_tokens **tokens);
void					handler(int signal);
void					handle_symlink(t_minishell **data, char *path,
							char *current_pwd);
char					*get_path(t_minishell *data);
void					free_split(char **split);
void					update_signal(t_minishell *data);
void					heredoc_signal(int signal);
void					handler_signal(int signal);
int						expand_verif(t_minishell **data, char *line);
t_tokens				*ft_tokennew_rework(char *value, t_token_type type);

#endif