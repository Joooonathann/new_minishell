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

char	*create_prompt(void)
{
	char	*result;

	result = "Temp: ";
	return (result);
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
	data->prompt = readline(create_prompt());
	if (data->prompt)
		data->tokens = parser(data->prompt, &data->env);
	else
		data->tokens = NULL;
	if (data->tokens)
		data->tokens_split = split_tokens(data->tokens);
	else
		data->tokens_split = NULL;
	data->current_tokens = NULL;
	return (data);
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
			printf("exit\n");
			break ;
		}
		if (data->tokens)
		{
			handler_exec(&data);
			ft_free_tokens(&data->tokens);
			free(data->prompt);
		}
	}
	return (0);
}
