/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   handler_exec.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jalbiser <jalbiser@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/19 02:09:36 by jalbiser          #+#    #+#             */
/*   Updated: 2024/09/20 07:17:05 by jalbiser         ###   ########.fr       */
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

t_tokens	*get_tokens_new(t_tokens *tokens)
{
	t_tokens	*result;

	result = NULL;
	while (tokens)
	{
		if (ft_strcmp(tokens->value, ">>") || ft_strcmp(tokens->value, "<<")
			|| ft_strcmp(tokens->value, "<") || ft_strcmp(tokens->value, ">"))
			tokens = tokens->next;
		else
			ft_tokenadd_back(&result, ft_tokennew(tokens->value));
		tokens = tokens->next;
	}
	return (result);
}

void	add_file(t_file **tokens, t_file *new)
{
	t_file	*tmp;

	if (tokens && new)
	{
		if (*tokens == NULL)
			*tokens = new;
		else
		{
			tmp = *tokens;
			while (tmp->next)
				tmp = tmp->next;
			tmp->next = new;
		}
	}
}

t_file	*new_file(char *value, char *type)
{
	t_file	*elem;

	elem = malloc(sizeof(t_file));
	if (!elem)
		return (NULL);
	elem->name = ft_strdup(value);
	elem->type = ft_strdup(type);
	elem->next = NULL;
	return (elem);
}

t_file	*get_files(t_tokens *tokens)
{
	t_file	*result;

	result = NULL;
	while (tokens)
	{
		if (ft_strcmp(tokens->value, ">>") || ft_strcmp(tokens->value, "<<")
			|| ft_strcmp(tokens->value, "<") || ft_strcmp(tokens->value, ">"))
		{
			if (tokens->next)
			{
				add_file(&result, new_file(tokens->next->value, tokens->value));
			}
			if (tokens->next && tokens->next->next)
				tokens = tokens->next->next;
			else
				break ;
		}
		else
		{
			tokens = tokens->next;
		}
	}
	return (result);
}

void fetch_redirection(t_minishell **data, t_tokens *tokens)
{
    struct stat file_stat;
    int fd;
    char *line;
    int heredoc_pipe[2];

    (*data)->files = get_files(tokens);
    while ((*data)->files)
    {
        if (ft_strcmp((*data)->files->type, "<<"))
        {
            if (pipe(heredoc_pipe) == -1)
            {
                perror("pipe");
                exit(EXIT_FAILURE);
            }
            while (1)
            {
                line = readline("> ");
                if (!line)
                {
                    close(heredoc_pipe[1]);
                    close(heredoc_pipe[0]);
                    exit(EXIT_FAILURE);
                }
                if (ft_strcmp(line, (*data)->files->name))
                {
                    free(line);
                    break;
                }
                write(heredoc_pipe[1], line, ft_strlen(line));
                write(heredoc_pipe[1], "\n", 1);
                free(line);
            }
            close(heredoc_pipe[1]);
            if (!(*data)->files->next)
                dup2(heredoc_pipe[0], STDIN_FILENO);
            close(heredoc_pipe[0]);
        }
        else if (ft_strcmp((*data)->files->type, "<"))
        {
            if (lstat((*data)->files->name, &file_stat) == -1)
            {
                perror("File does not exist");
                exit(EXIT_FAILURE);
            }
            if (!S_ISREG(file_stat.st_mode))
            {
                fprintf(stderr, "Error: %s is not a regular file\n", (*data)->files->name);
                exit(EXIT_FAILURE);
            }
            fd = open((*data)->files->name, O_RDONLY);
            if (fd < 0)
            {
                perror("Error opening file for reading");
                exit(EXIT_FAILURE);
            }
            if (fstat(fd, &file_stat) == 0 && file_stat.st_size == 0)
                dup2(fd, STDIN_FILENO);
            else
                dup2(fd, STDIN_FILENO);
            close(fd);
        }
        else if (ft_strcmp((*data)->files->type, ">"))
        {
            fd = open((*data)->files->name, O_WRONLY | O_CREAT | O_TRUNC, 0644);
            if (fd < 0)
            {
                perror("Error opening file for writing");
                exit(EXIT_FAILURE);
            }
            if (!(*data)->files->next)
                dup2(fd, STDOUT_FILENO);
            close(fd);
        }
        else if (ft_strcmp((*data)->files->type, ">>"))
        {
            fd = open((*data)->files->name, O_WRONLY | O_CREAT | O_APPEND, 0644);
            if (fd < 0)
            {
                perror("Error opening file for appending");
                exit(EXIT_FAILURE);
            }
            if (!(*data)->files->next)
                dup2(fd, STDOUT_FILENO);
            close(fd);
        }
        (*data)->files = (*data)->files->next;
    }
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
		(*data)->current_tokens = get_tokens_new((*data)->tokens_split[i]);
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
			fetch_redirection(data, (*data)->tokens_split[i]);
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
