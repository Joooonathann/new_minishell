/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec_system.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jalbiser <jalbiser@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/20 20:25:43 by jalbiser          #+#    #+#             */
/*   Updated: 2024/09/20 20:34:48 by jalbiser         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static void	handle_input_redirection(t_minishell **data)
{
	struct stat	file_stat;
	int			fd;

	if (lstat((*data)->files->name, &file_stat) == -1)
	{
		perror("File does not exist");
		exit(EXIT_FAILURE);
	}
	if (!S_ISREG(file_stat.st_mode))
	{
		fprintf(stderr, "Error: %s is not a regular file\n",
			(*data)->files->name);
		exit(0);
	}
	fd = open((*data)->files->name, O_RDONLY);
	if (fd < 0)
	{
		perror("Error opening file for reading");
		exit(0);
	}
	dup2(fd, STDIN_FILENO);
	close(fd);
}

static void	handle_output_redirection(t_minishell **data, int flags)
{
	int	fd;

	fd = open((*data)->files->name, flags, 0644);
	if (fd < 0)
	{
		perror("Error opening file");
		exit(EXIT_FAILURE);
	}
	if (!(*data)->files->next)
		dup2(fd, STDOUT_FILENO);
	close(fd);
}

void	fetch_redirection(t_minishell **data, t_tokens *tokens)
{
	(*data)->files = get_files(tokens);
	while ((*data)->files)
	{
		if (ft_strcmp((*data)->files->type, "<<"))
			handle_heredoc(data);
		else if (ft_strcmp((*data)->files->type, "<"))
			handle_input_redirection(data);
		else if (ft_strcmp((*data)->files->type, ">"))
			handle_output_redirection(data, O_WRONLY | O_CREAT | O_TRUNC);
		else if (ft_strcmp((*data)->files->type, ">>"))
			handle_output_redirection(data, O_WRONLY | O_CREAT | O_APPEND);
		(*data)->files = (*data)->files->next;
	}
}
