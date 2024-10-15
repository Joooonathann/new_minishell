/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   tokens_split.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jalbiser <jalbiser@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/19 01:37:42 by jalbiser          #+#    #+#             */
/*   Updated: 2024/10/15 16:30:58 by jalbiser         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	calculate_size_tokens(t_tokens *tokens)
{
	int	count;

	count = 1;
	while (tokens)
	{
		if (ft_strcmp(tokens->value, "|") && tokens->type == TYPE_PIPE)
			count++;
		tokens = tokens->next;
	}
	return (count);
}

t_tokens	**split_tokens(t_tokens *tokens)
{
	t_tokens	**result;
	int			i;

	i = 0;
	result = malloc(sizeof(t_tokens *) * (calculate_size_tokens(tokens) + 1));
	if (!result)
		return (NULL);
	result[i] = NULL;
	while (tokens)
	{
		if (ft_strcmp(tokens->value, "|") && tokens->type == TYPE_PIPE)
			result[++i] = NULL;
		else
		{
			ft_tokenadd_back(&result[i], ft_tokennew(tokens->value));
			result[i]->type = tokens->type;
		}
		tokens = tokens->next;
	}
	result[i + 1] = NULL;
	return (result);
}
