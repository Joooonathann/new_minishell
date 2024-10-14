/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   signal.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jalbiser <jalbiser@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/14 17:50:53 by jalbiser          #+#    #+#             */
/*   Updated: 2024/10/14 18:09:26 by jalbiser         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int		g_signal = 0;

void	handler(int signal)
{
	if (signal == SIGINT)
	{
		g_signal = 1;
		rl_on_new_line();
		rl_replace_line("", 0);
		ioctl(STDIN_FILENO, TIOCSTI, "\n");
	}
}

void	update_signal(t_minishell *data)
{
	if (g_signal)
	{
		update_vars(&data->env, "?", "130");
		g_signal = 0;
	}
}

void	handler_signal(int signal)
{
	if (signal == SIGINT)
	{
		g_signal = 2;
		write(0, "\n", 1);
	}
	else if (signal == SIGQUIT)
	{
		g_signal = 3;
		write(0, "\n", 1);
	}
}

void	execute_process(t_minishell **data)
{
	if ((*data)->tokens)
	{
		signal(SIGINT, handler_signal);
		signal(SIGQUIT, handler_signal);
		handler_exec(data);
		if (g_signal == 2)
		{
			update_vars(&(*data)->env, "?", "130");
			g_signal = 0;
		}
		else if (g_signal == 3)
		{
			update_vars(&(*data)->env, "?", "131");
			g_signal = 0;
		}
		signal(SIGINT, handler);
		clean_process(data, FALSE, FALSE);
	}
}

void	heredoc_signal(int signal)
{
	if (signal == SIGINT)
	{
		g_signal = 0;
		exit(130);
	}
	else if (signal == SIGQUIT)
	{
		g_signal = 0;
		exit(131);
	}
}
