/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   extern_command.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jalbiser <jalbiser@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/19 18:01:41 by jalbiser          #+#    #+#             */
/*   Updated: 2024/09/19 23:07:15 by jalbiser         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static int	get_count_vars(t_vars *env)
{
	int	i;

	i = 0;
	while (env)
	{
		i++;
		env = env->next;
	}
	return (i);
}

static char	**get_env(t_vars *env)
{
	char	**result;
	int		i;

	result = malloc(sizeof(char *) * (get_count_vars(env) + 1));
	if (!result)
		return (NULL);
	i = 0;
	while (env)
	{
		result[i] = malloc(sizeof(char) * (ft_strlen(env->key)
					+ ft_strlen(env->value) + 2));
		if (!result[i])
			return (NULL);
		ft_strcpy(result[i], env->key);
		ft_strcat(result[i], "=");
		ft_strcat(result[i], env->value);
		i++;
		env = env->next;
	}
	result[i] = NULL;
	return (result);
}

static char	*get_path(t_minishell *data)
{
	char	**splited_path;
	char	*full_path;
	t_vars	*tmp;
	int		i;

	tmp = get_vars(&data->env, "PATH");
	if (!tmp)
		return (NULL);
	splited_path = ft_split(tmp->value, ':');
	if (!splited_path)
		return (NULL);
	i = 0;
	while (splited_path[i])
	{
		full_path = malloc(sizeof(char) * (ft_strlen(splited_path[i])
					+ ft_strlen(data->current_tokens->value) + 2));
		if (!full_path)
			return (NULL);
		ft_strcpy(full_path, splited_path[i]);
		ft_strcat(full_path, "/");
		ft_strcat(full_path, data->current_tokens->value);
		if (access(full_path, X_OK) == 0)
			return (full_path);
		free(full_path);
		i++;
	}
	return (NULL);
}

static char	**get_args(t_tokens *tokens, char *command_path)
{
	char	**result;
	int		i;

	if (!tokens->next)
		return (NULL);
	tokens = tokens->next;
	result = malloc(sizeof(char *) * (ft_count_tokens(tokens) + 2));
	if (!result)
		return (NULL);
	result[0] = command_path;
	i = 1;
	while (tokens)
	{
		result[i++] = tokens->value;
		tokens = tokens->next;
	}
	result[i] = NULL;
	return (result);
}

void	extern_command(t_minishell **data)
{
	char		*command_path;
	char		**args;
	char		**envp;
	struct stat	file_stat;

	if (ft_strchr((*data)->current_tokens->value, '/'))
		command_path = (*data)->current_tokens->value;
	else
		command_path = get_path(*data);
	args = get_args((*data)->current_tokens, command_path);
	envp = get_env((*data)->env);
	if (!command_path)
	{
		fprintf(stderr, "%s: command not found\n", (*data)->current_tokens->value);
		exit(127);
	}
	if (lstat(command_path, &file_stat) == 0)
	{
		if (S_ISDIR(file_stat.st_mode))
		{
			fprintf(stderr, "%s: Is a directory\n", (*data)->current_tokens->value);
			exit(126);
		}
	}
	if (execve(command_path, args, envp) == -1)
	{
		if (errno == EACCES)
		{
			fprintf(stderr, "%s: Permission denied\n",
				(*data)->current_tokens->value);
			exit(126);
		}
		else if (errno == ENOENT)
		{
			fprintf(stderr, "%s: No such file or directory\n",
				(*data)->current_tokens->value);
			exit(127);
		}
		else
			exit(EXIT_FAILURE);
	}
	exit(0);
}
