/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec_func.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jalbiser <jalbiser@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/20 16:38:01 by jalbiser          #+#    #+#             */
/*   Updated: 2024/09/22 03:39:58 by jalbiser         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	handler(int signal)
{
	if (signal == SIGINT)
	{
		printf("\n");
		exit(130);
		return ;
	}
	else if (signal == SIGQUIT)
		return ;
}

void	handler_signal(int signal)
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
