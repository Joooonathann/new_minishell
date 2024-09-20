/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec_system.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jalbiser <jalbiser@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/20 16:36:28 by jalbiser          #+#    #+#             */
/*   Updated: 2024/09/20 16:36:37 by jalbiser         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	fetch_redirection(t_minishell **data, t_tokens *tokens)
{
	(*data)->files = get_files(tokens);
	while ((*data)->files)
	{
		handle_redirection((*data)->files);
		(*data)->files = (*data)->files->next;
	}
}

void	handle_redirection(t_file *file)
{
	if (ft_strcmp(file->type, "<<"))
		handle_heredoc(file);
	else if (ft_strcmp(file->type, "<"))
		handle_input_redirection(file);
	else if (ft_strcmp(file->type, ">"))
		handle_output_redirection(file, O_WRONLY | O_CREAT | O_TRUNC);
	else if (ft_strcmp(file->type, ">>"))
		handle_output_redirection(file, O_WRONLY | O_CREAT | O_APPEND);
}

void	handle_heredoc(t_file *file)
{
	int		heredoc_pipe[2];
	char	*line;

	if (pipe(heredoc_pipe) == -1)
		exit_perror("pipe");
	while (1)
	{
		line = readline("> ");
		if (!line || ft_strcmp(line, file->name))
			break ;
		write_heredoc_line(heredoc_pipe, line);
	}
	close(heredoc_pipe[1]);
	dup2(heredoc_pipe[0], STDIN_FILENO);
	close(heredoc_pipe[0]);
	free(line);
}
