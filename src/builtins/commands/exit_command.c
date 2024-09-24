/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exit_command.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jalbiser <jalbiser@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/20 00:18:11 by jalbiser          #+#    #+#             */
/*   Updated: 2024/09/24 03:21:00 by jalbiser         ###   ########.fr       */
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
		if (!ft_isdigit(*str))
			return (0);
		str++;
	}
	return (1);
}

void	exit_and_clean(t_minishell **data, int code)
{
	clean_process(data, TRUE, TRUE);
	exit(code);
}

void	exit_command(t_minishell **data)
{
	if (ft_count_tokens((*data)->current_tokens) == 1)
		exit_and_clean(data, 0);
	else if (!is_numeric_argument((*data)->current_tokens->next->value))
	{
		ft_error(3, "bash: exit:", (*data)->current_tokens->next->value,
			": numeric argument required");
		exit_and_clean(data, 2);
	}
	else if (ft_count_tokens((*data)->current_tokens) > 2)
	{
		ft_error(1, "bash: exit: too many arguments");
		return ;
	}
	else if (ft_atoi((*data)->current_tokens->next->value) > 2147483647)
	{
		ft_error(3, "bash: exit:", (*data)->current_tokens->next->value,
			": numeric argument required");
		exit_and_clean(data, 2);
	}
	else
		exit_and_clean(data, ft_atoi((*data)->current_tokens->next->value));
}
