/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec_redirection.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jalbiser <jalbiser@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/20 16:29:18 by jalbiser          #+#    #+#             */
/*   Updated: 2024/09/21 15:55:06 by jalbiser         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	write_to_heredoc_pipe(int fd, const char *line)
{
	write(fd, line, ft_strlen(line));
	write(fd, "\n", 1);
}

void	close_heredoc_pipe(int fd)
{
	close(fd);
}

void	read_heredoc_lines(t_minishell **data, int heredoc_pipe)
{
	char	*line;

	while (1)
	{
		line = readline("> ");
		if (!line)
		{
			close_heredoc_pipe(heredoc_pipe);
			exit(EXIT_FAILURE);
		}
		if (ft_strcmp(line, (*data)->files->name))
		{
			free(line);
			break ;
		}
		write_to_heredoc_pipe(heredoc_pipe, line);
		free(line);
	}
}

void	handle_heredoc(t_minishell **data)
{
	int	heredoc_pipe[2];

	if (pipe(heredoc_pipe) == -1)
	{
		perror("pipe");
		exit(EXIT_FAILURE);
	}
	read_heredoc_lines(data, heredoc_pipe[1]);
	close_heredoc_pipe(heredoc_pipe[1]);
	if (!(*data)->files->next)
		dup2(heredoc_pipe[0], STDIN_FILENO);
	close_heredoc_pipe(heredoc_pipe[0]);
}
