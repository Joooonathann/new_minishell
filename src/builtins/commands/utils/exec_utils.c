/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec_utils.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jalbiser <jalbiser@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/20 16:35:52 by jalbiser          #+#    #+#             */
/*   Updated: 2024/10/15 16:29:22 by jalbiser         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

t_tokens	*get_tokens_new(t_tokens *tokens)
{
	t_tokens	*result;

	result = NULL;
	while (tokens)
	{
		if (is_redirection(tokens))
			tokens = tokens->next;
		else
			ft_tokenadd_back(&result, ft_tokennew(tokens->value));
		tokens = tokens->next;
	}
	return (result);
}

int	is_redirection(t_tokens *tokens)
{
	if ((ft_strcmp(tokens->value, ">>") || ft_strcmp(tokens->value, "<<")
			|| ft_strcmp(tokens->value, "<") || ft_strcmp(tokens->value, ">"))
		&& tokens->type == TYPE_REDIRECTION)
		return (1);
	return (0);
}

void	add_file(t_file **tokens, t_file *new)
{
	t_file	*tmp;

	if (!tokens || !new)
		return ;
	if (*tokens == NULL)
		*tokens = new;
	else
	{
		tmp = *tokens;
		while (tmp->next)
			tmp = tmp->next;
		tmp->next = new;
	}
}

t_file	*new_file(char *value, char *type)
{
	t_file	*elem;

	elem = malloc(sizeof(t_file));
	if (!elem)
		return (NULL);
	elem->name = ft_strdup(value);
	elem->type = ft_strdup(type);
	elem->next = NULL;
	return (elem);
}

t_file	*get_files(t_tokens *tokens)
{
	t_file	*result;

	result = NULL;
	while (tokens)
	{
		if (is_redirection(tokens))
		{
			if (tokens->next)
				add_file(&result, new_file(tokens->next->value, tokens->value));
			if (tokens->next && tokens->next->next)
				tokens = tokens->next->next;
			else
				break ;
		}
		else
			tokens = tokens->next;
	}
	return (result);
}
