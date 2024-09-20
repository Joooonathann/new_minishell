/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   handler_exec.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jalbiser <jalbiser@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/19 02:09:36 by jalbiser          #+#    #+#             */
/*   Updated: 2024/09/20 00:44:14 by jalbiser         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	count_tokens_split(t_tokens **tokens)
{
	int	i;

	i = 0;
	while (tokens[i])
		i++;
	return (i);
}

int	nofork_command(t_tokens *tokens)
{
	if (ft_strcmp(tokens->value, "exit") || ft_strcmp(tokens->value, "cd")
		|| ft_strcmp(tokens->value, "unset") || ft_strcmp(tokens->value,
			"export"))
		return (1);
	return (0);
}

void	handler_exec(t_minishell **data)
{
	int		pipefd[2];
	pid_t	pid;
	int		i;
	int		prev_fd;
	int		status;
	char	*exit_code;

	i = 0;
	prev_fd = -1;
	while ((*data)->tokens_split[i])
	{
        (*data)->current_tokens = (*data)->tokens_split[i];
        if (count_tokens_split((*data)->tokens_split) == 1
		&& nofork_command((*data)->tokens))
        {
            handler_builtins(data);
            return ;
        }
		if (pipe(pipefd) == -1)
		{
			perror("pipe");
			return ;
		}
		pid = fork();
		if (pid == -1)
		{
			perror("fork");
			return ;
		}
		if (pid == 0)
		{
			if (prev_fd != -1)
			{
				dup2(prev_fd, STDIN_FILENO);
				close(prev_fd);
			}
			if ((*data)->tokens_split[i + 1])
				dup2(pipefd[1], STDOUT_FILENO);
			else if (prev_fd != -1)
				dup2(prev_fd, STDIN_FILENO);
			close(pipefd[0]);
			close(pipefd[1]);
			handler_builtins(data);
			exit(EXIT_SUCCESS);
		}
		else
		{
			close(pipefd[1]);
			if (prev_fd != -1)
				close(prev_fd);
			prev_fd = pipefd[0];
		}
		i++;
	}
	while (waitpid(-1, &status, 0) > 0)
	{
		if (WIFEXITED(status))
		{
			exit_code = ft_itoa(WEXITSTATUS(status));
			update_vars(&((*data)->env), "?", exit_code);
			free(exit_code);
		}
	}
	if (prev_fd != -1)
		close(prev_fd);
}
