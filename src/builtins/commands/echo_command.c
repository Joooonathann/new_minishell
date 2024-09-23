/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   echo_command.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jalbiser <jalbiser@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/19 16:00:41 by jalbiser          #+#    #+#             */
/*   Updated: 2024/09/24 00:24:12 by jalbiser         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	free_split_tokens(t_tokens **tokens)
{
	int	i;

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

static int	is_valid_option(const char *str)
{
	int	i;

	i = 1;
	if (str[0] != '-' || str[1] == '\0')
		return (0);
	while (str[i])
	{
		if (str[i] != 'n')
			return (0);
		i++;
	}
	return (1);
}

void	echo_command(t_minishell **data)
{
	int			is_valid;
	t_tokens	*tmp;

	tmp = (*data)->current_tokens;
	is_valid = 0;
	if (tmp->next)
	{
		tmp = tmp->next;
		while (is_valid_option(tmp->value))
		{
			is_valid = 1;
			tmp = tmp->next;
		}
		while (tmp)
		{
			printf("%s", tmp->value);
			if (tmp->next)
				printf(" ");
			tmp = tmp->next;
		}
	}
	if (!is_valid)
		printf("\n");
	exit(0);
}
