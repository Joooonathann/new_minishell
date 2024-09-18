/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   add_token_type.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ekrause <emeric.yukii@gmail.com>           +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/11 14:34:07 by ekrause           #+#    #+#             */
/*   Updated: 2024/09/11 14:34:19 by ekrause          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	add_token_type(t_tokens **tokens)
{
	t_tokens	*current_token;

	current_token = *tokens;
	while (current_token)
	{
		if ((ft_strlen(current_token->value) == 1
				|| (ft_strlen(current_token->value) == 2
					&& current_token->value[0] == current_token->value[1]))
			&& ft_strchr("><", current_token->value[0]))
			current_token->type = TYPE_REDIRECTION;
		else if (ft_strlen(current_token->value) == 1
			&& current_token->value[0] == '|')
			current_token->type = TYPE_PIPE;
		else if (!current_token->prev || (current_token->prev
				&& current_token->prev->type == TYPE_PIPE))
			current_token->type = TYPE_COMMAND;
		else if (current_token->value[0] && current_token->value[0] == '-'
			&& current_token->prev && (current_token->prev->type == TYPE_COMMAND
				|| current_token->prev->type == TYPE_OPTION))
			current_token->type = TYPE_OPTION;
		else
			current_token->type = TYPE_ARGUMENT;
		current_token = current_token->next;
	}
}
