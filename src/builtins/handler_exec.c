/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   handler_exec.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jalbiser <jalbiser@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/19 02:09:36 by jalbiser          #+#    #+#             */
/*   Updated: 2024/09/19 17:06:05 by jalbiser         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	handler_exec(t_minishell data)
{
	pid_t	pid;
	int		i;
	int		status;
	int		fd[2];
	int		in_fd;

	status = 0;
	in_fd = 0;
	i = 0;
	while (data.tokens_split[i])
	{
		if (data.tokens_split[i + 1] && pipe(fd) == -1)
        {
            perror("Pipe error");
            return ;
        }
		pid = fork();
		if (pid == -1)
		{
			perror("fork");
			return ;
		}
		else if (pid == 0)
		{
			if (in_fd != 0)
            {
                dup2(in_fd, STDIN_FILENO);
                close(in_fd);
            }
            if (data.tokens_split[i + 1] != NULL)
            {
                dup2(fd[1], STDOUT_FILENO);
            }
            close(fd[0]);
            close(fd[1]);
			data.current_tokens = data.tokens_split[i];
			handler_builtins(data);
			exit(0);
		}
		else
		{
			waitpid(pid, &status, 0);
			if (WIFEXITED(status))
				printf("Command exited with status: %d\n", WEXITSTATUS(status));
			else if (WIFSIGNALED(status))
				printf("Command terminated by signal: %d\n", WTERMSIG(status));
			close(fd[1]);
            in_fd = fd[0];
		}
		i++;
	}
	if (in_fd != 0)
        close(in_fd);
	return ;
}
