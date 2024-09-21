/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   handler_exec.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jalbiser <jalbiser@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/19 02:09:36 by jalbiser          #+#    #+#             */
/*   Updated: 2024/09/21 16:18:16 by jalbiser         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static void	handle_child(t_minishell **data, int prev_fd, int *pipefd, int i)
{
	fetch_redirection(data, (*data)->tokens_split[i]);
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

static void	handle_parent(int *pipefd, int *prev_fd)
{
	close(pipefd[1]);
	if (*prev_fd != -1)
		close(*prev_fd);
	*prev_fd = pipefd[0];
}

static void	wait_for_children(t_minishell **data)
{
	int		status;
	char	*exit_code;

	while (waitpid(-1, &status, 0) > 0)
	{
		if (WIFEXITED(status))
		{
			exit_code = ft_itoa(WEXITSTATUS(status));
			update_vars(&((*data)->env), "?", exit_code);
			free(exit_code);
		}
	}
}

static void	handle_pipe_and_fork(t_minishell **data, int *prev_fd, int i)
{
	int		pipefd[2];
	pid_t	pid;

	if (pipe(pipefd) == -1)
		perror("pipe");
	pid = fork();
	if (pid == -1)
		perror("fork");
	if (pid == 0)
		handle_child(data, *prev_fd, pipefd, i);
	handle_parent(pipefd, prev_fd);
}

void	handler_exec(t_minishell **data)
{
	int	i;
	int	prev_fd;

	i = 0;
	prev_fd = -1;
	while ((*data)->tokens_split[i])
	{
		(*data)->current_tokens = get_tokens_new((*data)->tokens_split[i]);
		if (count_tokens_split((*data)->tokens_split) == 1
			&& nofork_command((*data)->tokens))
			return (handler_builtins(data));
		handle_pipe_and_fork(data, &prev_fd, i);
		i++;
	}
	wait_for_children(data);
	if (prev_fd != -1)
		close(prev_fd);
}
