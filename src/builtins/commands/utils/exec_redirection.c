/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec_redirection.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jalbiser <jalbiser@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/20 16:29:18 by jalbiser          #+#    #+#             */
/*   Updated: 2024/10/15 10:45:22 by jalbiser         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	reparse(t_minishell **data)
{
	char	*prompt;

	prompt = compose_tokens((*data)->tokens);
	clean_process(data, FALSE, FALSE);
	(*data)->prompt = ft_strdup(prompt);
	free(prompt);
	if ((*data)->prompt)
		(*data)->tokens = parser((*data)->prompt, &(*data)->env);
	else
		(*data)->tokens = NULL;
	if ((*data)->tokens)
		(*data)->tokens_split = split_tokens((*data)->tokens);
	else
		(*data)->tokens_split = NULL;
	(*data)->files = NULL;
	(*data)->current_tokens = NULL;
}

void	write_to_heredoc_pipe(int fd, const char *line)
{
	write(fd, line, ft_strlen(line));
	write(fd, "\n", 1);
}

void	close_heredoc_pipe(int fd)
{
	close(fd);
}

int	expand_verif(t_minishell **data, char *line)
{
	char	**tmp;
	char	*tmp_filename;
	char	*tmp_filename_double;
	int		i;
	char	*tmp_line;
	char	*tmp_with_end_quote;
	char	*tmp_with_double_end_quote;

	tmp_line = ft_strdup(line);
	expand_var_heredoc(&tmp_line, &(*data)->env);
	if (ft_strcmp(tmp_line, (*data)->files->name))
		return (1);
	i = 0;
	tmp_filename = ft_strjoin("'", (*data)->files->name);
	if (!tmp_filename)
		return (0);
	tmp_with_end_quote = ft_strjoin(tmp_filename, "'");
	free(tmp_filename);
	tmp_filename = tmp_with_end_quote;
	tmp_filename_double = ft_strjoin("\"", (*data)->files->name);
	if (!tmp_filename_double)
	{
		free(tmp_filename);
		return (0);
	}
	tmp_with_double_end_quote = ft_strjoin(tmp_filename_double, "\"");
	free(tmp_filename_double);
	tmp_filename_double = tmp_with_double_end_quote;
	tmp = ft_split((*data)->prompt, ' ');
	if (!tmp)
	{
		free(tmp_filename);
		free(tmp_filename_double);
		return (0);
	}
	while (tmp[i])
	{
		if (ft_strcmp(tmp[i], tmp_filename) || ft_strcmp(tmp[i],
				tmp_filename_double))
		{
			free(tmp_filename);
			free(tmp_filename_double);
			free_split(tmp);
			return (0);
		}
		i++;
	}
	free(tmp_filename);
	free(tmp_filename_double);
	free_split(tmp);
	return (1);
}

void	read_heredoc_lines(t_minishell **data, int heredoc_pipe)
{
	char	*line;

	while (1)
	{
		line = readline("> ");
		if (!line)
			break ;
		if (expand_verif(data, line))
			expand_var_heredoc(&line, &(*data)->env);
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

	signal(SIGINT, heredoc_signal);
	signal(SIGQUIT, SIG_IGN);
	if (pipe(heredoc_pipe) == -1)
	{
		perror("pipe");
		exit(EXIT_FAILURE);
	}
	read_heredoc_lines(data, heredoc_pipe[1]);
	close_heredoc_pipe(heredoc_pipe[1]);
	if (is_last_heredoc(data))
	{
		(*data)->input_redirected = 1;
		dup2(heredoc_pipe[0], STDIN_FILENO);
	}
	close_heredoc_pipe(heredoc_pipe[0]);
}
