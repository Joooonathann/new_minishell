/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exit_command.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jalbiser <jalbiser@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/20 00:18:11 by jalbiser          #+#    #+#             */
/*   Updated: 2024/09/20 15:57:03 by jalbiser         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static int	is_numeric_argument(const char *str)
{
	if (*str == '\0')
		return (0);
	if (*str == '-' || *str == '+')
		str++;
	if (*str == '\0')
		return (0);
	while (*str)
	{
		if (!isdigit(*str))
			return (0);
		str++;
	}
	return (1);
}

void	exit_command(t_minishell **data)
{
	if (ft_count_tokens((*data)->current_tokens) == 1)
		exit(0);
	else if (!is_numeric_argument((*data)->current_tokens->next->value))
	{
		fprintf(stderr, "bash: exit: %s: numeric argument required\n",
			(*data)->current_tokens->next->value);
		exit(2);
	}
	else if (ft_count_tokens((*data)->current_tokens) > 2)
	{
		fprintf(stderr, "bash: exit: too many arguments\n");
		exit(1);
	}
	else if (ft_atoi((*data)->current_tokens->next->value) > 2147483647)
	{
		fprintf(stderr, "bash: exit: %s: numeric argument required\n",
			(*data)->current_tokens->next->value);
		exit(2);
	}
	else
		exit(ft_atoi((*data)->current_tokens->next->value));
}
