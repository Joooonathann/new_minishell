/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jalbiser <jalbiser@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/20 16:19:40 by jalbiser          #+#    #+#             */
/*   Updated: 2024/10/13 18:08:37 by jalbiser         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	handler(int signal)
{
	if (signal == SIGINT)
	{
		printf("\n");
		rl_on_new_line();
		rl_replace_line("", 0);
		rl_redisplay();
	}
	else if (signal == SIGQUIT)
		return ;
}

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

int	main(int argc, char **argv, char **envp)
{
	t_minishell	*data;
	
	data = malloc(sizeof(t_minishell));
	if (!data)
		return (1);
	data->env = init_vars(envp);
	(void)argv;
	(void)argc;
	signal(SIGINT, handler);
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
