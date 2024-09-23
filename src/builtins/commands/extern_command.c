/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   extern_command.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jalbiser <jalbiser@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/19 18:01:41 by jalbiser          #+#    #+#             */
/*   Updated: 2024/09/24 00:48:48 by jalbiser         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	free_split(char **split)
{
	int	i;

	if (!split)
		return ;
	i = 0;
	while (split[i])
	{
		free(split[i]);
		i++;
	}
	free(split);
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

static void	handle_command_errors(char *cmd_path)
{
	struct stat	file_stat;

	if (lstat(cmd_path, &file_stat) == 0 && S_ISDIR(file_stat.st_mode))
	{
		ft_error(2, cmd_path, ": Is a directory");
		exit(126);
	}
}

void	exec_command(char *cmd_path, char **args, char **envp)
{
	if (execve(cmd_path, args, envp) == -1)
	{
		if (errno == EACCES)
		{
			ft_error(2, cmd_path, ": Permission denied");
			exit(126);
		}
		else if (errno == ENOENT)
		{
			ft_error(2, cmd_path, ": No such file or directory");
			exit(127);
		}
		else
			exit(EXIT_FAILURE);
	}
}

void	extern_command(t_minishell **data)
{
	char		*command_path;
	char		**args;
	char		**envp;
	t_tokens	*tmp;

	tmp = (*data)->current_tokens;
	command_path = NULL;
	if (ft_strchr(tmp->value, '/'))
		command_path = tmp->value;
	else
		command_path = get_path(*data);
	if (!command_path)
	{
		ft_error(2, tmp->value, ": command not found");
		exit(127);
	}
	args = get_args(tmp, command_path);
	envp = get_env((*data)->env);
	handle_command_errors(command_path);
	exec_command(command_path, args, envp);
	exit(EXIT_SUCCESS);
}
