/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jalbiser <jalbiser@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/20 16:19:40 by jalbiser          #+#    #+#             */
/*   Updated: 2024/09/23 22:51:54 by jalbiser         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

#include <stdlib.h>
#include <string.h>
#include <unistd.h>

t_minishell	*init_data(char **envp, t_minishell *data)
{
	if (!data)
	{
		data = malloc(sizeof(t_minishell));
		if (!data)
			return (NULL);
		data->env = init_vars(envp);
	}
	data->prompt = readline("myfuckingbash@42$ ");
	if (data->prompt)
		data->tokens = parser(data->prompt, &data->env);
	else
		data->tokens = NULL;
	if (data->tokens)
		data->tokens_split = split_tokens(data->tokens);
	else
		data->tokens_split = NULL;
	data->input_redirected = 0;
	data->output_redirected = 0;
	data->files = NULL;
	data->current_tokens = NULL;
	return (data);
}

void	free_current(t_tokens **tokens)
{
	t_tokens	*previous_token;

	if (!tokens || !*tokens)
		return ;
	while (*tokens)
	{
		previous_token = *tokens;
		*tokens = (*tokens)->next;
		free(previous_token);
		previous_token = NULL;
	}
	*tokens = NULL;
}

void	free_split_tokens(t_tokens **tokens)
{
	int			i;

	i = 0;
	if (!tokens || !*tokens)
		return ;
	while (tokens[i])
	{
		free_current(&tokens[i]);
		i++;
	}
	free(tokens);
}

void	clean_process(t_minishell **data, t_bool env, t_bool data_free)
{
	if ((*data)->tokens)
		ft_free_tokens(&(*data)->tokens);
	if ((*data)->current_tokens)
		free_current(&(*data)->current_tokens);
	if ((*data)->tokens_split)
		free_split_tokens((*data)->tokens_split);
	if ((*data)->prompt)
		free((*data)->prompt);
	if ((*data)->env && env)
		delete_all_vars(&(*data)->env);
	if (*data && data_free)
		free(*data);
}

void	execute_process(t_minishell **data)
{
	if ((*data)->tokens)
	{
		signal(SIGINT, SIG_IGN);
		handler_exec(data);
		signal(SIGINT, handler_signal);
		clean_process(data, FALSE, FALSE);
	}
}

int	main(int argc, char **argv, char **envp)
{
	t_minishell	*data;

	data = NULL;
	(void)argv;
	(void)argc;
	signal(SIGINT, handler_signal);
	signal(SIGQUIT, SIG_IGN);
	while (1)
	{
		data = init_data(envp, data);
		if (!data->prompt)
		{
			clean_process(&data, TRUE, TRUE);
			printf("exit\n");
			break ;
		}
		if (*data->prompt)
			add_history(data->prompt);
		execute_process(&data);
	}
	return (0);
}
