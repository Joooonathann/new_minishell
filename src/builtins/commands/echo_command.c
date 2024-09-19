/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   echo_command.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jalbiser <jalbiser@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/19 16:00:41 by jalbiser          #+#    #+#             */
/*   Updated: 2024/09/20 00:00:36 by jalbiser         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

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
	int	is_valid;

	is_valid = 0;
	if ((*data)->current_tokens->next)
	{
		(*data)->current_tokens = (*data)->current_tokens->next;
		while (is_valid_option((*data)->current_tokens->value))
		{
			is_valid = 1;
			(*data)->current_tokens = (*data)->current_tokens->next;
		}
		while ((*data)->current_tokens)
		{
			printf("%s", (*data)->current_tokens->value);
			if ((*data)->current_tokens->next)
				printf(" ");
			(*data)->current_tokens = (*data)->current_tokens->next;
		}
	}
	if (!is_valid)
		printf("\n");
	exit(0);
}
