/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_count_value_tokens.c                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jalbiser <jalbiser@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/06/27 13:19:40 by jalbiser          #+#    #+#             */
/*   Updated: 2024/09/06 01:26:05 by jalbiser         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	ft_count_value_tokens(t_tokens *tokens)
{
	int	total_length;

	total_length = 0;
	while (tokens)
	{
		total_length += ft_strlen(tokens->value);
		tokens = tokens->next;
	}
	return (total_length);
}
