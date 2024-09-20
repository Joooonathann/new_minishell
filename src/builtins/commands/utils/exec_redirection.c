/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec_redirection.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jalbiser <jalbiser@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/20 16:29:18 by jalbiser          #+#    #+#             */
/*   Updated: 2024/09/20 16:39:30 by jalbiser         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	write_heredoc_line(int heredoc_pipe[2], char *line)
{
	write(heredoc_pipe[1], line, ft_strlen(line));
	write(heredoc_pipe[1], "\n", 1);
	free(line);
}

void	handle_input_redirection(t_file *file)
{
	struct stat	file_stat;
	int			fd;

	if (lstat(file->name, &file_stat) == -1 || !S_ISREG(file_stat.st_mode))
		exit_perror("Error: not a regular file");
	fd = open(file->name, O_RDONLY);
	if (fd < 0)
		exit_perror("Error opening file for reading");
	dup2(fd, STDIN_FILENO);
	close(fd);
}

void	handle_output_redirection(t_file *file, int flags)
{
	int	fd;

	fd = open(file->name, flags, 0644);
	if (fd < 0)
		exit_perror("Error opening file for writing");
	dup2(fd, STDOUT_FILENO);
	close(fd);
}

void	exit_perror(char *message)
{
	perror(message);
	exit(EXIT_FAILURE);
}
