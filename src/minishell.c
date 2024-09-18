/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jalbiser <jalbiser@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/28 13:05:49 by jalbiser          #+#    #+#             */
/*   Updated: 2024/09/19 01:51:19 by jalbiser         ###   ########.fr       */
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
}

char *create_prompt(void)
{
	char	*result = "Temp: ";
	return (result);
}

t_minishell	init_data(char **envp)
{
	t_minishell	result;

	result.env = init_vars(envp);
	result.prompt = readline(create_prompt());
	if (result.prompt)
		result.tokens = parser(result.prompt, &result.env);
	else
		result.tokens = NULL;
	if (result.tokens)
		result.tokens_split = split_tokens(result.tokens);
	else
		result.tokens_split = NULL;
	return (result);
}

int main(int argc, char **argv, char **envp)
{
	t_minishell	data;

	(void) argv;
	(void) argc;
	signal(SIGINT, handler);
	signal(SIGQUIT, handler);
	while (1)
	{
		data = init_data(envp);
		if (!data.prompt)
		{
			printf("exit\n");
			break;
		}
		int i = 0;
		while (data.tokens_split[i])
		{
			while (data.tokens_split[i])
			{
				printf("%s", data.tokens_split[i]->value);
				data.tokens_split[i] = data.tokens_split[i]->next;
			}
			printf("\n");
			i++;
		}
		ft_free_tokens(&data.tokens);
		free(data.prompt);
	}
	return (0);
}