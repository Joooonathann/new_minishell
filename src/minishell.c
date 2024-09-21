/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jalbiser <jalbiser@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/20 16:19:40 by jalbiser          #+#    #+#             */
/*   Updated: 2024/09/22 01:36:28 by jalbiser         ###   ########.fr       */
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

int	main(int argc, char **argv, char **envp)
{
	t_minishell	*data;

	data = NULL;
	(void)argv;
	(void)argc;
	signal(SIGINT, handler);
	signal(SIGQUIT, handler);
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
		if (data->tokens)
		{
			handler_exec(&data);
			clean_process(&data, FALSE);
		}
	}
	return (0);
}
