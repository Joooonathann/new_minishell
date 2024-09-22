/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   compose_tokens.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jalbiser <jalbiser@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/22 02:52:21 by jalbiser          #+#    #+#             */
/*   Updated: 2024/09/22 18:13:44 by jalbiser         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	is_last_heredoc(t_minishell **data)
{
	t_file	*tmp;

	tmp = (*data)->files;
	if (!tmp->next)
		return (1);
	tmp = tmp->next;
	while (tmp)
	{
		if (ft_strcmp(tmp->type, "<<"))
			return (0);
		tmp = tmp->next;
	}
	return (1);
}

static int	calculate_total_length(t_tokens *tokens)
{
	int	total_len;

	total_len = 0;
	while (tokens)
	{
		total_len += ft_strlen(tokens->value);
		if (tokens->next)
			total_len++;
		tokens = tokens->next;
	}
	return (total_len);
}

static void	copy_tokens(char *result, t_tokens *tokens)
{
	int	i;
	int	z;

	i = 0;
	while (tokens)
	{
		z = 0;
		while (tokens->value[z])
			result[i++] = tokens->value[z++];
		if (tokens->next)
			result[i++] = ' ';
		tokens = tokens->next;
	}
	result[i] = '\0';
}

char	*compose_tokens(t_tokens *tokens)
{
	char	*result;
	int		total_len;

	total_len = calculate_total_length(tokens);
	result = malloc(sizeof(char) * (total_len + 1));
	if (!result)
		return (NULL);
	copy_tokens(result, tokens);
	return (result);
}
