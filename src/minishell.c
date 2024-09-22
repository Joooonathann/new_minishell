/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jalbiser <jalbiser@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/20 16:19:40 by jalbiser          #+#    #+#             */
/*   Updated: 2024/09/22 03:39:20 by jalbiser         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

char	*create_prompt(t_vars *env)
{
	char	*hostname;
	char	*prompt;
	char	buffer[1024];

	if (gethostname(buffer, sizeof(buffer)) == 0)
		hostname = buffer;
	else
		hostname = "42";
	prompt = ft_strjoin_three(get_vars(&env, "USER")->value, "@", hostname);
	prompt = ft_strjoin_free(prompt, "$ ");
	return (prompt);
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
	data->prompt_value = create_prompt(data->env);
	data->prompt = readline(data->prompt_value);
	if (data->prompt)
		data->tokens = parser(data->prompt, &data->env);
	else
		data->tokens = NULL;
	if (data->tokens)
		data->tokens_split = split_tokens(data->tokens);
	else
		data->tokens_split = NULL;
	data->files = NULL;
	data->current_tokens = NULL;
	return (data);
}

void	clean_process(t_minishell **data, t_bool env)
{
	if ((*data)->tokens)
		ft_free_tokens(&(*data)->tokens);
	if ((*data)->tokens_split)
		free((*data)->tokens_split);
	if ((*data)->prompt)
		free((*data)->prompt);
	if ((*data)->prompt_value)
		free((*data)->prompt_value);
	if ((*data)->env && env)
		delete_all_vars(&(*data)->env);
}

void	execute_process(t_minishell **data)
{
	if ((*data)->tokens)
	{
		signal(SIGINT, SIG_IGN);
		handler_exec(data);
		signal(SIGINT, handler_signal);
		clean_process(data, FALSE);
	}
}

int	main(int argc, char **argv, char **envp)
{
	t_minishell	*data;

	data = NULL;
	(void)argv;
	(void)argc;
	signal(SIGINT, handler_signal);
	signal(SIGQUIT, handler_signal);
	while (1)
	{
		data = init_data(envp, data);
		if (!data->prompt)
		{
			clean_process(&data, TRUE);
			printf("exit\n");
			break ;
		}
		if (*data->prompt)
			add_history(data->prompt);
		execute_process(&data);
	}
	return (0);
}
